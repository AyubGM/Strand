import os
import subprocess
import platform


from SetupPython import PythonConfiguration as PythonRequirements
# Make sure everything we need is installed
PythonRequirements.Validate()

from Vulkan import VulkanConfiguration as VulkanRequirements

# Change from Scripts directory to root
os.chdir('./../') # Change from devtools/scripts directory to root

VulkanRequirements.Validate()

if platform.system() == "Windows":
    print("\nRunning premake...")
    subprocess.call([os.path.abspath("./scripts/Win-GenProjects.bat"), "nopause"])

print("\nSetup completed!")