#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Strand/Core/Application.h"
//#include "Strand/Core/Application.cpp"

// --- Minimal Stubs for Dependencies ---
// In a real project, you would #include the actual headers.
// These are minimal definitions just to make the test file compile.

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <filesystem>
#include <iostream>






namespace Strand {

	// This "Friend" class inherits from Application and exposes private members/methods
	// for testing. This is a common pattern for testing C++ classes.
	class FriendApplication : public Application {
	public:
		FriendApplication(const ApplicationSpecification& spec) : Application(spec) {}

		// Expose private state for testing
		bool IsMinimized() const { return m_Minimized; }
		bool IsRunning() const { return m_Running; }

		// Expose private methods for testing
		void TestExecuteMainThreadQueue() {
			ExecuteMainThreadQueue();
		}

		// Expose event handlers
		bool TestOnWindowClose(WindowCloseEvent& e) {
			return OnWindowClose(e);
		}

		bool TestOnWindowResize(WindowResizeEvent& e) {
			return OnWindowResize(e);
		}
	};

	// GTest fixture for setting up and tearing down the application
	class ApplicationTest : public ::testing::Test {
	protected:
		std::unique_ptr<FriendApplication> m_App;

		void SetUp() override {
			Strand::Log::Init();
			// Reset the singleton instance before each test
			Application::s_Instance = nullptr;

			ApplicationSpecification spec;
			spec.Name = "Test Application";

			// This will create the app, create the MockWindow,
			// and set Application::s_Instance
			m_App = std::make_unique<FriendApplication>(spec);
		}

		void TearDown() override {
			m_App.reset();
			Application::s_Instance = nullptr;
		}
	};

	// The client-defined CreateApplication function (required to link)
	Application* CreateApplication(ApplicationCommandLineArgs args) {
		// Not used in these tests, but must exist
		return nullptr;
	}


	// --- The Tests ---

	TEST_F(ApplicationTest, Constructor) {
		// Test that the constructor sets the singleton instance
		ASSERT_NE(m_App, nullptr);
		EXPECT_EQ(&Application::Get(), m_App.get());

		// Test that the app is running by default
		EXPECT_TRUE(m_App->IsRunning());
	}

	TEST_F(ApplicationTest, OnWindowClose) {
		WindowCloseEvent e;
		EXPECT_TRUE(m_App->IsRunning());

		// Simulate dispatching the event
		m_App->TestOnWindowClose(e);

		// Test that the handler sets m_Running to false
		EXPECT_FALSE(m_App->IsRunning());
	}

	TEST_F(ApplicationTest, OnWindowResizeMinimized) {
		// Initial state should not be minimized
		EXPECT_FALSE(m_App->IsMinimized());

		// Simulate a minimize event
		WindowResizeEvent e(0, 0);
		m_App->TestOnWindowResize(e);

		// Test that the app state is now minimized
		EXPECT_TRUE(m_App->IsMinimized());
	}

	TEST_F(ApplicationTest, OnWindowResizeRestored) {
		// First, set the state to minimized
		WindowResizeEvent e_min(0, 0);
		m_App->TestOnWindowResize(e_min);
		ASSERT_TRUE(m_App->IsMinimized());

		// Now, simulate a restore (non-zero size) event
		WindowResizeEvent e_restore(1280, 720);
		m_App->TestOnWindowResize(e_restore);

		// Test that the app state is no longer minimized
		EXPECT_FALSE(m_App->IsMinimized());
	}

	TEST_F(ApplicationTest, MainThreadQueue) {
		bool function1_executed = false;
		int function2_value = 0;

		// Submit two functions to the main thread queue
		m_App->SubmitToMainThread([&]() {
			function1_executed = true;
			});

		m_App->SubmitToMainThread([&]() {
			function2_value = 42;
			});

		// At this point, the functions should not have executed
		EXPECT_FALSE(function1_executed);
		EXPECT_EQ(function2_value, 0);

		// Manually run the queue
		m_App->TestExecuteMainThreadQueue();

		// Now, check that both functions have executed
		EXPECT_TRUE(function1_executed);
		EXPECT_EQ(function2_value, 42);
	}

	TEST_F(ApplicationTest, MainThreadQueueIsCleared) {
		int execute_count = 0;

		m_App->SubmitToMainThread([&]() {
			execute_count++;
			});

		// Run the queue
		m_App->TestExecuteMainThreadQueue();
		ASSERT_EQ(execute_count, 1);

		// Run the queue again, it should be empty
		m_App->TestExecuteMainThreadQueue();

		// Check that the function was not executed a second time
		EXPECT_EQ(execute_count, 1);
	}

} // namespace Strand

// Standard GTest main function
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}