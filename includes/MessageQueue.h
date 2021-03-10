#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <queue>

enum MessageType
{
	MT_FileChanged
};
class Message
{
public:
	MessageType messageType;
};

class MessageFileModified : public Message
{
public:
	std::string filename;
	std::string extension;
	FileStatus fileStatus;
};

class MessageQueue
{
private:
	std::queue<Message*> queueMessages;
	inline static MessageQueue* current = nullptr;
public:

	static MessageQueue* Create()
	{
		if (current != nullptr) delete current;

		current = new MessageQueue();
		return current;
	}

	static MessageQueue* Current()
	{
		return current;
	}

	void Notify_FileChanged(std::string filename, std::string extension, FileStatus fileStatus)
	{
		MessageFileModified* message = new MessageFileModified();
		message->messageType = MT_FileChanged;
		message->filename = filename;
		message->extension = extension;
		message->fileStatus = fileStatus;

		queueMessages.push(message);
	}
	void Handle_FileChanged(MessageFileModified* messageFileModified)
	{
		switch (messageFileModified->fileStatus)
		{
		case FileStatus::created:
			std::cout << "[FileWatcher] File created: " << messageFileModified->filename << '\n';
			break;
		case FileStatus::modified:
			std::cout << "[FileWatcher] File modified: " << messageFileModified->filename << '\n';

			if (messageFileModified->extension == "glsl")
			{
				// The file is a shader so we need to Hot Swap or recompile the shader.
				ShaderProgram::RecompileShader(messageFileModified->filename);
			}

			break;
		case FileStatus::erased:
			std::cout << "[FileWatcher] File erased: " << messageFileModified->filename << '\n';
			break;
		default:
			std::cout << "[FileWatcher] Error! Unknown file status.\n";
		}
	}

	void update()
	{
		while(!queueMessages.empty())
		{
			Message* message = queueMessages.front();
			queueMessages.pop();

			switch(message->messageType)
			{
			case MessageType::MT_FileChanged:
				Handle_FileChanged((MessageFileModified*)message);
				break;
			}

			delete message;
		}
	}
};

#endif