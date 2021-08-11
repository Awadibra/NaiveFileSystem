#include "FileSystem.h"
#include <iostream>
#include "Commands.h"
using namespace std;

FileSystem::FileSystem():rootDirectory(new Directory("",0)),workingDirectory(rootDirectory) {

}

Directory& FileSystem::getRootDirectory() const {
    return *rootDirectory;
}

Directory& FileSystem::getWorkingDirectory() const {
    return *workingDirectory;
}

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory=newWorkingDirectory;
}

FileSystem::~FileSystem() {
    if((verbose==1)|(verbose==3)){
        cout<<"FileSystem::~FileSystem()"<<endl;
    }
    delete(rootDirectory);
}

FileSystem::FileSystem(const FileSystem &other):rootDirectory(other.rootDirectory),workingDirectory() {
    if((verbose==1)|(verbose==3)){
        cout<<"FileSystem::FileSystem(const FileSystem &other)"<<endl;
    }
    (this->rootDirectory)=(other.rootDirectory);
    string s=other.workingDirectory->getAbsolutePath();
    CdCommand Pointer(s);
    Pointer.execute((FileSystem &) *this);

}

FileSystem::FileSystem( FileSystem &&other):rootDirectory(other.rootDirectory),workingDirectory(){
    if((verbose==1)|(verbose==3)){
        cout<<"FileSystem::FileSystem( FileSystem &&other)"<<endl;
    }
    string s=" " + other.getWorkingDirectory().getAbsolutePath();
    this->rootDirectory=new Directory (std::move(other.getRootDirectory()));
    delete(other.rootDirectory);
    this->workingDirectory=new Directory("",0);
    CdCommand* cd=new CdCommand(s);
    cd->execute(*this);
}

FileSystem& FileSystem::operator=( FileSystem&& other){
    if((verbose==1)|(verbose==3)){
        cout<<"FileSystem::FileSystem( FileSystem &&other)"<<endl;
    }
    if(&other!=this){
       delete(this->workingDirectory);
        delete(this->rootDirectory);
        this->rootDirectory=new Directory(std::move(*other.rootDirectory));
        this->rootDirectory->setParentsForChildren(this->rootDirectory);
        string s=other.workingDirectory->getAbsolutePath();
        CdCommand Pointer("cd "+s);
        Pointer.execute((FileSystem &) *this);
        delete(other.rootDirectory);

    }
    return *this;
}

FileSystem& FileSystem::operator=(const FileSystem &other) {
    if((verbose==1)|(verbose==3)){
        cout<<"FileSystem& FileSystem::operator=(const FileSystem &other)"<<endl;
    }
    if(this!=&other){
      delete(this->workingDirectory);
        delete(this->rootDirectory);
        this->rootDirectory=new Directory(*other.rootDirectory);
        string s=other.workingDirectory->getAbsolutePath();
        CdCommand Pointer(s);
        Pointer.execute((FileSystem &) *this);
    }
    return *this;
}
