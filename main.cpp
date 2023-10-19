#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <filesystem>
#include <unistd.h>
#include <pwd.h>
#include <ctype.h>

// Handle invalid/bad input from std::cin
void ignoreLine()
{
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Shorted function version for invalid/bad input handling
void badInputHandler()
{
   if(std::cin.eof()){
      std::cout << "EXITED!" << std::endl;
      std::exit(0);
   }

   std::cin.clear();	// Go back to 'normal' operation
   ignoreLine();		// Remove bad input
}

void createFile(const std::string_view filename)
{
   std::ofstream file(filename.data());
   if(file){
      std::cout << "\nFile " << filename << " successfully created." << std::endl;
      file.close();
   } else {
      std::cerr << "\nFailed to create file." << std::endl;
   }
}

void listFilesInDirectory(const std::string_view directory){
   std::cout << std::endl;
   for(const auto& entry: std::filesystem::directory_iterator(directory))
   {
      std::cout << entry.path().filename() << '\n';
   }
   std::cout << std::endl;
}

void deleteFile(const std::string_view filename)
{
   if(std::filesystem::remove(filename)){
      std::cout << "\n\nFile " << filename << " deleted successfully" << std::endl;
   } else {
      std::cerr << "\n\nFailed to delete file." << std::endl;
   }
}

int main()
{
   uid_t uid = geteuid();
   struct passwd* pwd = getpwuid(uid);
   
   bool loopHanlder{ true };
   
   while(loopHanlder)
   {
      std::cout << "File Management System\n";
      std::cout << "1. Create File\n";
      std::cout << "2. List Files in Directory\n";
      std::cout << "3. Delete File\n";
      std::cout << "4. Exit\n";
      std::cout << "Enter  your choice: ";

      int choice{};
      std::cin >> choice;
      if(std::cin.fail()){
         badInputHandler();
      }

      switch(choice)
      {
         case 1:{
            std::string filename;
            std::cout << "Enter the filename: ";
            std::cin >> filename;
            if(std::cin.fail()){
               badInputHandler();
            }
            createFile(filename);
         }break;

         case 2:{
            // gets userID of current user
            uid_t uid = geteuid();
            struct passwd* pwd = getpwuid(uid);
            
            std::string directory;
            std::string extra;
            std::string homeDir{};
            if(pwd != nullptr) { homeDir = pwd->pw_dir; }
            std::cout << "Enter the directory path: ";
            std::cin >> directory;
            extra = directory;
            if(directory[0] == '~'){
               directory.erase(0);
               directory.append(homeDir);
               extra.erase(extra.begin());
               directory.append(extra);
            }
            
            if(std::cin.fail()){
               badInputHandler();
            }
            if(std::filesystem::exists(directory))
            {
               listFilesInDirectory(directory);
            } else {
               std::cout << "Directory does not exists...\n\n";
               std::cout << directory << '\n';
            }
            
         }break;

         case 3:{
            std::string filename;
            char safetyCheck{};
            std::cout << "Enter filename to delete: ";
            std::cin >> filename;
            if(std::cin.fail()){
               badInputHandler();
            }

            if(std::filesystem::exists(filename))
            {
               do{
                  std::cout << "Are you sure you want to delete " << filename << "? (Y/N): ";
                  std::cin >> safetyCheck;
                  if(std::cin.fail()){
                     badInputHandler();
                  }
               }while (std::toupper(safetyCheck) != 'Y' && std::toupper(safetyCheck) != 'N');
            } else {
               std::cout << "\nFILE DOES NOT EXISTS!\n" << std::endl;
               break;
            }
            
            if(std::toupper(safetyCheck) == 'Y'){
               deleteFile(filename);
            } else if (std::toupper(safetyCheck) == 'N'){
               std::cout << "\nTERMINATION ABORTED!" << std::endl;
            }
         } break;

         case 4:{
            std::cout << "Exiting File management System. Goodbye!" << std::endl;
            loopHanlder = false ;
         }break;

         default:{
            std::cout << "Invalid choice. Please select a valid option!\n\n";
         }break;
      }
   }
   
   return 0;
}
