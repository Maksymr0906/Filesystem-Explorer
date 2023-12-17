#include <iostream>
#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

enum class Option {
	COPY = 1,
	MOVE,
	DELETE,
	QUIT
};

void instruction() {
	std::cout << "\n1 - Copy" << std::endl
			  << "2 - Move" << std::endl
			  << "3 - Delete" << std::endl
			  << "4 - Quit" << std::endl << std::endl;      
}

Option getOption() {
	instruction();
	int choice{};
	std::cout << "Enter a command: ";
	std::cin >> choice;
	return static_cast<Option>(choice);
}

void listFiles(const fs::path& directory) {
	try {
		if (fs::exists(directory)) {
			if (fs::is_regular_file(directory)) {
				std::cout << directory << " is a file." << std::endl;
			}
			else if (fs::is_directory(directory)) {
				std::cout << directory << " is a directory containing:\n";
				for (fs::directory_entry& item : fs::directory_iterator(directory)) {
					std::cout << item.path() << std::endl;
				}
			}
		}
		else {
			std::cout << directory << " does not exist\n";
		}
	}
	catch (const fs::filesystem_error& ex) {
		std::cout << ex.what() << std::endl;
	}
}

void copyFile(const fs::path& sourcePath, const fs::path& destinationPath) {
	fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
	std::cout << "File copied successfully." << std::endl;
}

void moveFile(const fs::path& sourcePath, const fs::path& destinationPath) {
	fs::rename(sourcePath, destinationPath);
	std::cout << "File moved successfully." << std::endl;
}

void deleteFile(const fs::path& filePath) {
	if (fs::exists(filePath)) {
		fs::remove(filePath);
		std::cout << "File deleted: " << filePath.filename() << std::endl;
	}
	else {
		std::cout << "File not found: " << filePath.filename() << std::endl;
	}
}

int main() {
	std::string pathStr{};
	for (;;) {
		std::cout << "Enter directory path: ";
		std::getline(std::cin, pathStr);
		fs::path currentPath{ pathStr };
		listFiles(currentPath);

		Option option = getOption();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (option == Option::QUIT) {
			return 0;
		}

		std::cout << "Enter file name: ";
		std::string fileName{};
		std::getline(std::cin, fileName);

		fs::path sourcePath = currentPath / fileName;

		if (!fs::exists(sourcePath) || !fs::is_regular_file(sourcePath)) {
			std::cerr << "File not found or is not a regular file: " << sourcePath.filename() << std::endl;
			continue;
		}

		std::string destinationDir{};
		if (option == Option::COPY || option == Option::MOVE) {
			std::cout << "Enter destination directory: ";
			std::getline(std::cin, destinationDir);
		}
		fs::path destinationPath = fs::absolute(fs::path(destinationDir)) / fileName;

		switch (option) {
		case Option::COPY:
			copyFile(sourcePath, destinationPath);
			break;
		case Option::MOVE:
			moveFile(sourcePath, destinationPath);
			break;
		case Option::DELETE:
			deleteFile(sourcePath);
			break;
		default:
			std::cerr << "Invalid choice. Try again." << std::endl;
			break;
		}
	}
}