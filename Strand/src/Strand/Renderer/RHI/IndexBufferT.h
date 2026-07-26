namespace Strand {

	class IndexBufferT
	{
	public:
		~IndexBufferT() {}

		void Bind() const;
		void Unbind() const = 0;

		uint32_t GetCount() const = 0;
	};
}