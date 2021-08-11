#include "GlobalVariables.h"
#include <Files.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;

bool nameComp(BaseFile *f1,BaseFile *f2) {
    return ( f1->getName().compare(f2->getName())<0);
}

bool sizeComp(BaseFile *f1,BaseFile *f2) {
    return ( f1->getSize()-f2->getSize()<0);
}

BaseFile::BaseFile(string name): name(name) {

}

BaseFile::~BaseFile() {

}

void BaseFile::setName(string newName) {
    this->name=newName;
}

string BaseFile::getName() const {
    return name;
}


File::File( string name, int size) : BaseFile(name),size(size) {

}

int File::getSize() {
    return this->size;
}

bool File::type() {
    return true;
}


Directory::Directory(string name, Directory *parent): BaseFile(name),children(),  parent(parent) {

}

Directory* Directory::getParent() const {
    return parent;
}

void Directory::setParent(Directory *newParent) {
    this->parent=newParent;
}

void Directory::addFile(BaseFile *file) {
    children.push_back(file);
}

void Directory::removeFile(string name) {

    for(size_t i=0;i<this->getChildren().size();i++){

        if(this->getChildren().at(i)->getName().compare(name)==0){
            delete(children.at(i));
            children.erase(children.begin()+i);
        }
    }
}
void Directory::removeFile(BaseFile *file) {
    removeFile(file->getName());

}

void Directory::sortByName() {
    sort(children.begin(),children.end(),nameComp);
}

void Directory::sortBySize() {
    sort(children.begin(),children.end(),sizeComp);
}

vector<BaseFile*> Directory::getChildren() {
    return this->children;
}

int Directory::getSize() {
    int size=0;
    for(size_t i=0;i<children.size();i++){
        if(children.at(i)->type()){
            size=size+((File*)(children.at(i)))->getSize();
        }
        else{
            size=size+((Directory*)(children.at(i)))->getSize();
        }
    }
    return size;
}

string Directory::getAbsolutePath() {
    if(parent!=0){
        string path="";
        Directory* temp = parent;
        while(temp !=0){
            path=temp->getName()+"/"+path;
            temp = temp->getParent();
        }
        return path+this->getName();
    }
    else{
        return "/";
    }
}

bool Directory::type() {
    return false;
}

bool Directory::check(string s) {
    for(size_t i=0;i<children.size();i++){
        if(!children.at(i)->type() && children.at(i)->getName().compare(s) == 0){
            return true;
        }
    }
    return false;
}


bool Directory::checkF(string s) {
    for(size_t i=0;i<children.size();i++){
        if( children.at(i)->getName().compare(s)==0){
            return true;
        }
    }
    return false;
}

Directory::~Directory() {
    if((verbose==1)|(verbose==3)){
        cout<<"Directory::~Directory()"<<endl;
    }
    while(this->getChildren().size()>0){
        this->removeFile(getChildren().at(0));
    }
    children.clear();
    children.shrink_to_fit();
}

Directory::Directory(const Directory &other):BaseFile(other.getName()),children(),parent(nullptr) {
    if((verbose==1)|(verbose==3)){
        cout<<"Directory::Directory(const Directory &other)"<<endl;
    }
    for(size_t i=0;i<other.children.size();i++){
        if(other.children.at(i)->type()){
             children.push_back(new File((other.children.at(i)->getName()),(other.children.at(i)->getSize())));
        }
        else{
            Directory* fs=new Directory((Directory&)*other.children.at(i));
            fs->setParent(this);
            this->children.push_back((Directory*)(fs));
        }

    }

}

Directory::Directory(Directory &&other):BaseFile(other.getName()),children(other.children),parent(nullptr) {
    if((verbose==1)|(verbose==3)){
        cout<<"Directory::Directory(Directory &&other)"<<endl;
    }
    other.children.clear();
    other.parent->removeChild(other);
    setParentsForChildren(this);

}

void Directory::setParentsForChildren(Directory *parent) {
    for (size_t i=0;i<parent->getChildren().size();i++){
        if (!parent->getChildren().at(i)->type()){
            ((Directory*) parent->children.at(i))->setParent(parent);
        }
    }
}
void Directory::removeChild(Directory dir) {
    for (size_t i=0;i<this->getChildren().size();i++){
        if(this->getChildren().at(i)->getName().compare(dir.getName())==0){
            removeFile(getChildren().at(i));
        }
    }
}

Directory& Directory::operator=(const Directory& other) {
    if((verbose==1)|(verbose==3)){
        cout<<"Directory& Directory::operator=(const Directory& other)"<<endl;
    }
    if( (this)!=&other){
        this->setName(other.getName());
        this->setParent(other.getParent());
        this->Copythechildren(other);

    }
    return *this;
}


void Directory::Copythechildren( Directory s) {

    for(size_t i=0;i<s.children.size();i++){
        if(s.children.at(i)->type()){
            this->children.push_back(new File(*(dynamic_cast<File *>(s.children.at(i)))));}
        else{
            this->children.push_back(new Directory(*(dynamic_cast<Directory*>(s.children.at(i)))));}
    }
}


Directory &Directory::operator=(Directory &&other) {
    if((verbose==1)|(verbose==3)){
        cout<<"Directory &Directory::operator=(Directory &&other)"<<endl;
    }
    if(this!=&other){

        while(children.size()>0){
            children.erase(children.begin());
        }
        children.clear();
        children.shrink_to_fit();
        while(other.children.size()>0){
            BaseFile* pointer=other.children.at(0);
            other.children.erase(children.begin());
            this->children.push_back(pointer);
        }
        other.children.clear();
        other.children.shrink_to_fit();
        delete(&other);

    }
    return *this;
}



