#ifndef LOGS_HPP
#define LOGS_HPP

// This is not getting patched, stop spreading misinformation...
class Logs
{
public:
	std::uint64_t GetRenderView();
}; static Logs* logs = new Logs;

#endif // LOGS_HPP