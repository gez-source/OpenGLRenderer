#include "FileWatcher.h"
#include "TimeManager.h"
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

// Keep a record of files from the base directory and their last modification time
FileWatcher::FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay, std::function<void(std::string, FileStatus)>& action) : path_to_watch{ path_to_watch }, delay{ delay }
{
	this->action_ = action;
	for (auto& file : std::filesystem::recursive_directory_iterator(path_to_watch)) 
	{
	     paths_[file.path().string()] = std::filesystem::last_write_time(file);
	}
}

FileWatcher::~FileWatcher()
{
	stop();
}

void FileWatcher::stop()
{
	// to exit thread gracefully 
	listening_ = false;
	update_th0->join();
	delete update_th0;
}

// Monitor "path_to_watch" for changes and in case of a change execute the user supplied "action" function
void FileWatcher::start()
{
	// do other stuff in parallel on main thread while running this file watcher thread.
	update_th0 = new std::thread(update_thread, std::ref(listening_), update_interval, path_to_watch, std::ref(paths_), action_);
}

void update_thread(std::atomic<bool>& program_is_running, unsigned int update_interval_millisecs,
	std::string path_to_watch, std::unordered_map<std::string, std::filesystem::file_time_type>& paths_, std::function<void(std::string, FileStatus)> action_)
{
	const auto wait_duration = std::chrono::milliseconds(update_interval_millisecs);
	while (program_is_running)
	{
		watch(path_to_watch, paths_, action_);

		std::this_thread::sleep_for(wait_duration);
	}
}

void watch(std::string path_to_watch, std::unordered_map<std::string, std::filesystem::file_time_type>& paths_, std::function<void(std::string, FileStatus)> action_)
{
	auto it = paths_.begin();
	while (it != paths_.end())
	{
		if (!std::filesystem::exists(it->first))
		{
			action_(it->first, FileStatus::erased);
			it = paths_.erase(it);
		}
		else
		{
			++it;
		}
	}

	// Check if a file was created or modified
	for (auto& file : std::filesystem::recursive_directory_iterator(path_to_watch))
	{
		auto current_file_last_write_time = std::filesystem::last_write_time(file);
		std::string filePath = file.path().string();
		if (!contains(paths_, filePath))
		{
			// File creation
			paths_[filePath] = current_file_last_write_time;
			action_(filePath, FileStatus::created);
		}
		else
		{
			// File modification
			if (paths_[filePath] != current_file_last_write_time)
			{
				paths_[filePath] = current_file_last_write_time;
				action_(filePath, FileStatus::modified);
			}
		}
	}
}

// Check if "paths_" contains a given key
// If your compiler supports C++20 use paths_.contains(key) instead of this function
bool contains(std::unordered_map<std::string, std::filesystem::file_time_type> paths_, const std::string& key)
{
	auto el = paths_.find(key);
	return el != paths_.end();
}