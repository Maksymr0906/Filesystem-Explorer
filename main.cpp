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

void list_files(const fs::path& directory) {
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

int main() {
	std::string path_str{};
	for (;;) {
		std::cout << "Enter directory path: ";
		std::getline(std::cin, path_str);
		fs::path current_path{ path_str };
		list_files(current_path);
		Option option = getOption();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (option == Option::COPY) {
			std::cout << "Enter source file name: ";
			std::string source_file;
			std::getline(std::cin, source_file);

			fs::path source_path = current_path / source_file;

			if (!fs::exists(source_path) || !fs::is_regular_file(source_path)) {
				std::cerr << "File not found or is not a regular file: " << source_path.filename() << std::endl;
				continue;
			}

			std::cout << "Enter destination directory: ";
			std::string destination_dir;
			std::getline(std::cin, destination_dir);

			fs::path destination_path = fs::absolute(fs::path(destination_dir)) / source_file;
			fs::copy_file(source_path, destination_path, fs::copy_options::overwrite_existing);
			std::cout << "File copied successfully." << std::endl;
		}
		else if (option == Option::MOVE) {
			std::cout << "Enter source file name: ";
			std::string source_file;
			std::getline(std::cin, source_file);

			fs::path source_path = current_path / source_file;

			if (!fs::exists(source_path) || !fs::is_regular_file(source_path)) {
				std::cerr << "File not found or is not a regular file: " << source_path.filename() << std::endl;
				continue;
			}

			std::cout << "Enter destination directory: ";
			std::string destination_dir;
			std::getline(std::cin, destination_dir);

			fs::path destination_path = fs::absolute(fs::path(destination_dir)) / source_file;
			fs::rename(source_path, destination_path);
			std::cout << "File moved successfully." << std::endl;
		}
		else if (option == Option::DELETE) {
			std::cout << "Enter file name to delete: ";
			std::string file_name;
			std::getline(std::cin, file_name);

			fs::path file_path = current_path / file_name;

			if (fs::exists(file_path)) {
				fs::remove(file_path);
				std::cout << "File deleted: " << file_path.filename() << std::endl;
			}
			else {
				std::cout << "File not found: " << file_path.filename() << std::endl;
			}
		}
		else if (option == Option::QUIT) {
			return 0;
		}
		else {
			std::cerr << "Invalid choice. Try again." << std::endl;
			break;
		}
	}
}