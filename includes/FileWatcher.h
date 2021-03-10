#ifndef FILE_WATCHER_H
#define FILE_WATCHER_H

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <atomic>

// Available file changes.
enum class FileStatus { created, modified, erased };

// Threaded file watching functions.
bool contains(std::unordered_map<std::string, std::filesystem::file_time_type> paths_, const std::string& key);
void watch(std::string path_to_watch, std::unordered_map<std::string, std::filesystem::file_time_type>& paths_, std::function<void(std::string, FileStatus)> action_);
void update_thread(std::atomic<bool>& program_is_running, unsigned int update_interval_millisecs,
	std::string path_to_watch, std::unordered_map<std::string, std::filesystem::file_time_type>& paths_, std::function<void(std::string, FileStatus)> action_);

class FileWatcher 
{
private:
	const unsigned int update_interval = 1000; // update interval in milliseconds
	std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
	std::function<void(std::string, FileStatus)> action_;
	std::thread* update_th0;
	std::atomic<bool> listening_{ true };
public:
	std::string path_to_watch;
	// Time interval at which we check the base folder for changes
	std::chrono::duration<int, std::milli> delay;

	// Keep a record of files from the base directory and their last modification time
	FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay, std::function<void(std::string, FileStatus)>& action);
	~FileWatcher();

	// Stop monitoring the file system for changes.
	void stop();

	// Monitor "path_to_watch" for changes and in case of a change execute the user supplied "action" function
	void start();
};

#endif