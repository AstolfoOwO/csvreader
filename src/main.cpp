#include "../include/csv.h"

using namespace std;
using fpath = std::filesystem::path;

int main(int argc, char** argv) {
    setlocale(LC_ALL, "Russian");
    
    if (argc <= 1) {
        cerr << "Недостаточно аргументов.\n Используйте: " << argv[0] << " <filepath> <query>\n";
        return 1; 
    }
	
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)  {
		cerr << "Используйте: " << argv [0] << " <filepath> <query> \n";
		return 0;
	}

    
	fpath path = argv[1];
	string query = argv[2];

    Csv file(path);
	file.execute(query);
	file.commit();

	return 0; 
}
