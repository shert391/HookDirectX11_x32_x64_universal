class Hook {
private:
	byte originalBytesOne[15] = { 0 };
	byte originalBytesTwo[14] = { 0 };

	int lenPathOne = 0;
	int lenPathTwo = 0;

	void* locationOne = nullptr;
	void* locationTwo = nullptr;

	void* allocedOne = nullptr;
	void* allocedTwo = nullptr;
public:
	void* hook1(void* src, OUT void* var);
	void* hook2(void* src, void* dst);
	void Unhook();
};
