#ifndef ENVIRxONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"
#include <string>
#include <vector>

using namespace std;

class Environment {
private:
    vector<BaseCommand*> commandsHistory;
    FileSystem fs;

public:
    virtual ~Environment();
    Environment(const Environment& other);
    Environment(Environment&& other);
    Environment& operator=(const Environment& other);
    Environment& operator=(Environment&& other);


    void CopyVector(vector<BaseCommand*> other);
    Environment();
    void removecommands(vector<BaseCommand*> s);
    void start();
    FileSystem& getFileSystem() ; // Get a reference to the file system
    void addToHistory(BaseCommand *command); // Add a new command to the history
    const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
};

#endif