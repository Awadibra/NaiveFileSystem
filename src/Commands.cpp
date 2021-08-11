#include <Commands.h>


using namespace std;

BaseCommand::BaseCommand(string args): args(args) {
}

string BaseCommand::getArgs() {
    return args;
}

BaseCommand::~BaseCommand() {}

vector<string>* BaseCommand::path(string s) {
    s=s+'/';
    vector<string> *v=new vector<string>;
    string word="";
    if(s.at(0)=='/'){s=s.substr(1);}

    for(size_t i=0;i<s.size();i++){
        if(s.at(i)!='/'){
            word=word+s.at(i);
        }
        else{
            v->push_back(word);
            word="";
        }
    }
    return v;
}



PwdCommand::PwdCommand(string args) : BaseCommand(args) {

}

void PwdCommand::execute(FileSystem &fs) {
    std::cout << fs.getWorkingDirectory().getAbsolutePath()<< std::endl;

}

string PwdCommand::toString() {
    return "pwd";
}

CdCommand::CdCommand(string args) : BaseCommand(args) {

}

void CdCommand::execute(FileSystem &fs) {
    string s=this->getArgs().substr(this->getArgs().find(' ')+1);
    vector<string> *v=this->path(s);
    Directory *dir;
    bool found=true;
    if (s.length()==1 && s.at(0)=='/'){
        fs.setWorkingDirectory(&fs.getRootDirectory());
    }
    else{
        if(s.at(0)=='/'){
            dir=&fs.getRootDirectory();
        }
        else {
            dir=&fs.getWorkingDirectory();
        }
        for (size_t i= 0;(i<v->size()) & (found) ; i++) {
            if(v->at(i).compare("..")==0){
                if(dir->getParent()== 0){
                    found=false;
                    cout << "The system cannot find the path specified" << endl;
                }
                else{
                    dir=dir->getParent();
                }
            }
            else if(dir->check(v->at(i))){
                for (size_t j=0 ; j<dir->getChildren().size();j++) {
                    if (!dir->getChildren().at(j)->type() && dir->getChildren().at(j)->getName().compare(v->at(i)) == 0) {
                        dir = (Directory *) dir->getChildren().at(j);
                    }
                }
            }
            else{
                found=false;
                cout<<"The system cannot find the path specified"<<endl;
            }

        }
        if(found){
            fs.setWorkingDirectory(dir);
        }
    }
    delete(v);
}

string CdCommand::toString() {
    return "cd";
}


LsCommand::LsCommand(string args) : BaseCommand(args) {

}

void LsCommand::execute(FileSystem &fs) {
    string s=this->getArgs().substr(this->getArgs().find(' ')+1);
    bool bySize=false;
    if(s.substr(0,2).compare("-s")==0){
        bySize=true;
        s=s.substr(s.find(' ')+1);
    }
    vector<string> *v=this->path(s);
    Directory *dir;
    bool found=true;
    if(s.at(0)!='/'){
        dir=&fs.getWorkingDirectory();

        if(s.compare("ls")!=0 && s.compare("-s")!=0 ){
            for (size_t i= 0;(i<v->size()) & (found) ; i++) {
                if(v->at(i).compare("..")==0){
                    if(dir->getParent()== 0){
                        cout << "The system cannot find the path specified" << endl;
                        found=false;
                    }
                    else{
                        dir=dir->getParent();
                    }
                }
                else if(dir->check(v->at(i))){
                    for (size_t j=0 ; j<dir->getChildren().size();j++) {
                        if (!dir->getChildren().at(j)->type() && dir->getChildren().at(j)->getName().compare(v->at(i)) == 0) {
                            dir = (Directory *) dir->getChildren().at(j);
                        }
                    }
                }
                else{
                    found=false;
                    cout<<"The system cannot find the path specified"<<endl;
                }
            }
        }
        if(found){
            dir->sortByName();
            if (bySize){
                dir->sortBySize();
            }
            for(size_t i=0; i<dir->getChildren().size();i++){
                if(dir->getChildren().at(i)->type()==false){

                    cout<<"DIR"<<"\t"<<dir->getChildren().at(i)->getName()<<"\t"<<dir->getChildren().at(i)->getSize()<<endl;
                }
                else{
                    cout<<"FILE"<<"\t"<<dir->getChildren().at(i)->getName()<<"\t"<<dir->getChildren().at(i)->getSize()<<endl;
                }
            }
        }
    }
    else{
        dir=&fs.getRootDirectory();
        for (size_t i= 0;(i<v->size()) & (found) ; i++) {
            if(v->at(i).compare("..")==0){
                if(dir->getParent()== 0){
                    cout << "The system cannot find the path specified" << endl;
                }
                else{
                    dir=dir->getParent();
                }
            }
            else if(dir->check(v->at(i))){
                for (size_t j=0 ; j<dir->getChildren().size();j++) {
                    if (!dir->getChildren().at(j)->type() && dir->getChildren().at(j)->getName().compare(v->at(i)) == 0) {
                        dir = (Directory *) dir->getChildren().at(j);
                    }
                }
            }
            else if (s.size()==1 && s.at(0)=='/'){
                found=true;
            }
            else{
                found=false;
                cout<<"The system cannot find the path specified"<<endl;
            }

        }

        if(found){
            dir->sortByName();
            if (bySize){
                dir->sortBySize();
            }
            for(size_t i=0; i<dir->getChildren().size();i++){
                if(dir->getChildren().at(i)->type()==false){

                    cout<<"DIR"<<"\t"<<dir->getChildren().at(i)->getName()<<"\t"<<dir->getChildren().at(i)->getSize()<<endl;
                }
                else{
                    cout<<"FILE"<<"\t"<<dir->getChildren().at(i)->getName()<<"\t"<<dir->getChildren().at(i)->getSize()<<endl;
                }
            }
        }
     }
    delete(v);
}


string LsCommand::toString() {
    return "ls";
}

MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {

}

void MkdirCommand::execute(FileSystem &fs){

    string s=this->getArgs().substr(this->getArgs().find(' ')+1);
    vector<string> *v=this->path(s);
    Directory *dir;
    bool fileCheck=false;
    if(s.at(0)!='/'){
        dir=&fs.getWorkingDirectory();
    }
    else{
        dir=&fs.getRootDirectory();
    }
    for (size_t i =0;i<v->size() && !fileCheck;i++){
        if(dir->checkF(v->at(i))){
            for(size_t j =0 ; j<dir->getChildren().size() && !fileCheck ; j++){
                if (dir->getChildren().at(j)->getName().compare(v->at(i))==0){
                    if(dir->getChildren().at(j)->type()){
                        fileCheck=true;
                    }
                    else{
                        if (i==v->size()-1){
                            cout<<"The directory already exists"<<endl;

                        }
                        dir=(Directory*)dir->getChildren().at(j);
                    }
                }
            }
        }
        else{
            dir->addFile(new Directory(v->at(i), dir));
            dir=(Directory*)dir->getChildren().back();
        }
    }
    if (fileCheck){
        cout<<"The directory already exists"<<endl;
    }
    delete(v);
}

string MkdirCommand::toString() {
    return "mkdir";
}


MkfileCommand::MkfileCommand(string args) : BaseCommand(args) {

}

void MkfileCommand::execute(FileSystem &fs) {
    string s=this->getArgs().substr(this->getArgs().find(' ')+1);
    vector<string> *v=this->path(s);
    Directory *dir;
    if(s.at(0)=='/'){
        dir=&fs.getRootDirectory();
    }
    else{
        dir=&fs.getWorkingDirectory();
    }

    string fileName=(string)v->back();
    string sizeName= fileName.substr(fileName.find(' ')+1);
    int size = std::stoi(sizeName);
    fileName=fileName.substr(0,fileName.find(' '));
    v->pop_back();
    bool check1=false;
    bool check=true;
    if(v->size()==0){
        check1=true;
    }
    for (size_t i=0;(i<v->size()) & (check); i++){
        check=false;

        for(size_t z=0; z<dir->getChildren().size();z++){
            if((!dir->getChildren().at(z)->type()) & (dir->getChildren().at(z)->getName().compare(v->at(i))==0) ){
                dir=(Directory*) dir->getChildren().at(z);
                check=true;
            }


        }
        if((i == v->size() - 1) & check){
            check1=true;
        }
    }
    if (check1==true){
        if(!dir->checkF(fileName)) {
            dir->addFile(new File(fileName, size));
        }
        else{ cout<< "File already exists"<<endl;}
    }
    else{
        cout<< "The system cannot find the path specified"<<endl;
    }
    delete(v);
}




string MkfileCommand::toString() {
    return "mkfile";
}

RenameCommand::RenameCommand(string args) : BaseCommand(args) {

}

string RenameCommand::toString() {
    return "rename";
}

void RenameCommand::execute(FileSystem &fs) {
    string s = this->getArgs().substr(this->getArgs().find(' ') + 1);
    vector<string> *v = this->path(s);
    Directory *dir;
    if (s.at(0) == '/') {
        dir = &fs.getRootDirectory();
    }
    else {
        dir = &fs.getWorkingDirectory();
    }
    string fileName = (string) v->back();
    string newName = fileName.substr(fileName.find(' ') + 1);
    string previousName = fileName.substr(0, fileName.find(' '));
    v->pop_back();
    bool check=true;
    size_t i=0;
    for ( i = 0; (i < v->size()) & check; i++) {
        check=false;
        if(v->at(i).compare("..")==0){
            if(dir->getParent()!=0){
                dir=dir->getParent();
                check=true;
            }
            else{
                check=false;
            }
        }
        else {
            for (size_t z = 0; z < dir->getChildren().size(); z++) {

                if ((!dir->getChildren().at(z)->type()) & (dir->getChildren().at(z)->getName().compare(v->at(i)) == 0)) {
                    dir = (Directory *) dir->getChildren().at(z);
                    check = true;
                }
            }
        }
        if(!check){ cout<<"No such file or directory"<<endl;}
    }

    if((i==v->size()) & check){
        if(dir->getAbsolutePath().compare("/")==0 &&(dir->getAbsolutePath() + previousName).compare(fs.getWorkingDirectory().getAbsolutePath())==0){
            cout<<"Can't rename the working directory"<<endl;
        }

        else  if((dir->getAbsolutePath() + "/" + previousName).compare(fs.getWorkingDirectory().getAbsolutePath())==0){
            cout<<"Can't rename the working directory"<<endl;}
        else if(!dir->checkF(previousName)){
            cout<<"No such file or directory"<<endl;
        }
        else if (!dir->checkF(newName)){
            for(size_t i=0;i<dir->getChildren().size();i++){
               if( dir->getChildren().at(i)->getName().compare(previousName)==0){
                   dir->getChildren().at(i)->setName(newName);
               }
            }
        }
    }
    delete(v);
}

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> &history)
        :BaseCommand(args),history(history) {
}

void HistoryCommand::execute(FileSystem &fs) {
    for (size_t i=0; i < history.size(); i++) {
        cout << i << "\t" << history.at(i)->getArgs() << endl;
    }
}

string HistoryCommand::toString() {
    return "history";
}


RmCommand::RmCommand(string args) : BaseCommand(args) {

}


string RmCommand::toString() {
    return "rm";
}

void RmCommand::execute(FileSystem &fs) {
    string s = this->getArgs().substr(this->getArgs().find(' ') + 1);
    vector<string> *v = this->path(s);
    Directory *dir;
    if (s.at(0) == '/') {
        dir = &fs.getRootDirectory();
    }
    else {
        dir = &fs.getWorkingDirectory();
    }
    if(s.at(0)=='/' && fs.getWorkingDirectory().getAbsolutePath().substr(0,s.size()).compare(s)==0){
        cout<<"Can't remove directory"<<endl;
    }
    else{
        bool found=false;
        if (v->size()==1 && dir->checkF(v->front())){
            found=true;
        }
        else{
            for (size_t i=0;i<v->size()-1;i++){
                found=false;
                for (size_t j=0;j<dir->getChildren().size();j++){
                    if(!found && !dir->getChildren().at(j)->type()&& v->at(i).compare(dir->getChildren().at(j)->getName())==0){
                        dir=(Directory*)dir->getChildren().at(j);
                        found=true;
                    }
                }
            }
        }
        if(!found){
            cout<<"No such file or directory"<<endl;
        }
        else if (!dir->checkF(v->at(v->size()-1))){
            cout<<"No such file or directory"<<endl;
        }
        else{
            for (size_t i= 0;  i<dir->getChildren().size() ; i++) {
                if(dir->getChildren().at(i)->getName().compare(v->at(v->size()-1))==0
                   && !dir->getChildren().at(i)->type()){
                    dir->removeFile(dir->getChildren().at(i));
                }
                else if(dir->getChildren().at(i)->getName().compare(v->at(v->size()-1))==0
                        && dir->getChildren().at(i)->type()){
                    dir->removeFile(dir->getChildren().at(i));


                }
            }
        }
    }
    delete(v);
}


CpCommand::CpCommand(string args) : BaseCommand(args) {

}

string CpCommand::toString() {
    return "cp";
}

void CpCommand::execute(FileSystem &fs) {
    string s = this->getArgs().substr(this->getArgs().find(' ') + 1);
    string sourcepath = s.substr(0,s.find(' ') );
    string destination = s.substr(s.find(' ') + 1);
    vector<string> *sourcePathVector = this->path(sourcepath);
    vector<string> *destinationPathVector = this->path(destination);
    Directory *dir;
    Directory *dirDest;
    BaseFile* Copy = 0;
    bool found = true;

    if (sourcepath.at(0) == '/') {
        dir = &fs.getRootDirectory();
    } else {
        dir = &fs.getWorkingDirectory();
    }

    for (size_t i = 0; (i < sourcePathVector->size()) & (found); i++) {
        if (sourcePathVector->at(i).compare("..") == 0) {
            if (dir->getParent() == 0) {
                cout << "No such file or directory" << endl;
                found=false;
            }
            else {
                dir = dir->getParent();
            }
        }
        else if (dir->checkF(sourcePathVector->at(i))) {
            for (size_t j = 0; j < dir->getChildren().size(); j++) {

                if (dir->getChildren().at(j)->type() &&
                    dir->getChildren().at(j)->getName().compare(sourcePathVector->at(i)) == 0
                    && i == sourcePathVector->size() - 1) {
                    Copy = new File(*(dynamic_cast<File *>(dir->getChildren().at(j))));
                }
                else if (!dir->getChildren().at(j)->type() &&
                           dir->getChildren().at(j)->getName().compare(sourcePathVector->at(i)) == 0) {
                    dir = (Directory *) dir->getChildren().at(j);
                }
            }
        }
        else{
            found=false;
            cout << "No such file or directory" << endl;
        }
    }
    if(found){
        bool found2=true;
        if (destination.at(0) == '/') {
            dirDest = &fs.getRootDirectory();
        } else {
            dirDest = &fs.getWorkingDirectory();
        }
        for (size_t i = 0; (i < destinationPathVector->size()) & (found2); i++) {
            if (destinationPathVector->at(i).compare("..") == 0) {
                if (dirDest->getParent() == 0) {
                    cout << "No such file or directory" << endl;
                    found2=false;
                }
                else {
                    //dir = (Directory *) dir->getChildren().at(i);
                    dirDest = dirDest->getParent();
                    if (i==destinationPathVector->size()-1){
                        if(Copy==0 ){
                           if(!dirDest->checkF(dir->getName())) {
                               Directory *Copy = new Directory(*dir);

                               dirDest->addFile(Copy);
                               Copy->setParent(dirDest);
                           }
                            else{ found2=false;
                           }
                        }
                        else{
                          if(!dirDest->checkF(Copy->getName())) {
                          dirDest->addFile(Copy);
                          }
                            else{
                              found2=false;
                          }
                        }
                    }
                }
            }
            else if (dirDest->checkF(destinationPathVector->at(i))) {
                for (size_t j = 0; j < dirDest->getChildren().size(); j++) {
                    if (!dirDest->getChildren().at(j)->type() &&
                        dirDest->getChildren().at(j)->getName().compare(destinationPathVector->at(i)) == 0
                        && i == destinationPathVector->size() - 1) {
                        if(Copy==0){
                            dirDest=(Directory *)dirDest->getChildren().at(j);
                            if(!dirDest->checkF(dir->getName())) {
                                Directory *Copy = new Directory(*dir);
                                dirDest->addFile(Copy);
                                Copy->setParent(dirDest);
                            }
                            else{ found2=false;}
                        }
                        else{
                            dirDest=(Directory*)dirDest->getChildren().at(j);
                            if(!dirDest->checkF(dir->getName())){
                            dirDest->addFile(Copy);}
                            else{found2=false;}
                        }
                    }
                    else if (!dirDest->getChildren().at(j)->type() &&
                             dirDest->getChildren().at(j)->getName().compare(destinationPathVector->at(i)) == 0) {
                        dirDest = (Directory *) dirDest->getChildren().at(j);
                    }
                }
            }
            else if((destination.length()==1) & (destination.at(0)=='/')){
                if(Copy==0){
                    dirDest=&fs.getRootDirectory();
                    if(!dirDest->checkF(dir->getName())){
                    Directory*  Copy=new Directory(*dir);
                    dirDest->addFile(Copy);
                    Copy->setParent(dirDest);}
                    else{found2=false;}
                }
                else{
                    if(!dirDest->checkF(dir->getName())){
                    dirDest->addFile(Copy);} else{found2=false;}
                }

            }
            else{
                found2=false;
                cout << "No such file or directory" << endl;
            }
            if ( !found2 && Copy!=0){
                delete(Copy);
            }
        }
    }

    delete(destinationPathVector);
    delete(sourcePathVector);
}

MvCommand::MvCommand(string args) : BaseCommand(args) {

}

void MvCommand::execute(FileSystem &fs) {
    string s = this->getArgs().substr(this->getArgs().find(' ') + 1);
    string sourcepath = s.substr(0,s.find(' ') );
    string destination = s.substr(s.find(' ') + 1);
    vector<string> *sourcePathVector = this->path(sourcepath);
    vector<string> *destinationPathVector = this->path(destination);
    int d=-1;
    Directory *dir;
    Directory *dirDest;
    BaseFile* Copy = 0;
    bool found = true;
    bool used=false;
    if (sourcepath.at(0) == '/') {
        dir = &fs.getRootDirectory();
    } else {
        dir = &fs.getWorkingDirectory();
    }

    for (size_t i = 0; (i < sourcePathVector->size()) & (found); i++) {
        if (sourcePathVector->at(i).compare("..") == 0) {
            if (dir->getParent() == 0) {
                cout << "No such file or directory" << endl;

                found=false;
            }
            else {
                dir = dir->getParent();
            }
        }
        else if (dir->checkF(sourcePathVector->at(i))) {
            for (size_t j = 0; j < dir->getChildren().size(); j++) {

                if (dir->getChildren().at(j)->type() &&
                    dir->getChildren().at(j)->getName().compare(sourcePathVector->at(i)) == 0
                    && i == sourcePathVector->size() - 1) {

                    Copy = new File(std::move(*(dynamic_cast<File *>(dir->getChildren().at(j)))));
                    d=j;

                }
                else if (!dir->getChildren().at(j)->type() &&
                         dir->getChildren().at(j)->getName().compare(sourcePathVector->at(i)) == 0) {
                    dir = (Directory *) dir->getChildren().at(j);
                }
            }
        }
        else{
            found=false;
            cout << "No such file or directory" << endl;
        }
    }



    if(found) {
        bool found2=true;
        if (Copy!=0 || (dir->getAbsolutePath().compare(fs.getWorkingDirectory().getAbsolutePath().substr(0,dir->getAbsolutePath().size())) != 0)
            & ((dir->getAbsolutePath().compare(fs.getRootDirectory().getAbsolutePath()) != 0))) {
            if (destination.at(0) == '/') {
                dirDest = &fs.getRootDirectory();
            } else {
                dirDest = &fs.getWorkingDirectory();
            }
            for (size_t i = 0; (i < destinationPathVector->size()) & (found2); i++) {
                if (destinationPathVector->at(i).compare("..") == 0) {
                    if (dirDest->getParent() == 0) {
                        cout << "No such file or directory" << endl;
                        found2 = false;
                    } else {
                        dirDest = dirDest->getParent();
                        if (i==destinationPathVector->size()-1) {

                            if (Copy == 0) {
                                if (!dirDest->checkF(dir->getName())) {
                                    Copy = new Directory(std::move(*dir));
                                    dirDest->addFile(Copy);
                                    ((Directory *) Copy)->setParent(dirDest);
                                    used=true;

                                } else {
                                    found2 = false;
                                }
                            } else {
                                if (!dirDest->checkF(dir->getName())) {
                                    dirDest->addFile(Copy);
                                    dir->removeFile(dir->getChildren().at(d));
                                    used=true;
                                } else {
                                    found2 = false;
                                    delete (Copy);
                                }
                            }
                        }
                        }
                    }
                else if (dirDest->checkF(destinationPathVector->at(i))) {
                    for (size_t j = 0; j < dirDest->getChildren().size(); j++) {
                        if (!dirDest->getChildren().at(j)->type() &&
                            dirDest->getChildren().at(j)->getName().compare(destinationPathVector->at(i)) == 0
                            && i == destinationPathVector->size() - 1) {
                            if (Copy == 0) {
                                dirDest = (Directory *) (dirDest->getChildren().at(j));
                                if(!dirDest->checkF(dir->getName())) {
                                    Copy = new Directory(std::move(*dir));
                                    dirDest->addFile(Copy);
                                    ((Directory *) Copy)->setParent(dirDest);
                                    ((Directory *) Copy)->setParentsForChildren((Directory *) Copy);
                                    used=true;

                                }
                                else{

                                    found2=false;
                                }
                            }
                            else {
                                dirDest = (Directory *) (dirDest->getChildren().at(j));
                                if(!dirDest->checkF(dir->getName())) {
                                     dirDest->addFile(Copy);
                                    dir->removeFile(dir->getChildren().at(d));
                                    used=true;

                                }
                                else{
                                    found2=false;
                                    delete(Copy);
                                }
                            }
                        }
                        else if (!dirDest->getChildren().at(j)->type() &&
                                   dirDest->getChildren().at(j)->getName().compare(destinationPathVector->at(i)) == 0) {
                            dirDest = (Directory *) dirDest->getChildren().at(j);
                        }
                    }
                } else if ((destination.at(0) == '/' )& (destination.length() == 1)) {
                    dirDest = &fs.getRootDirectory();
                    if (Copy == 0) {
                        if (!dirDest->checkF(dir->getName())) {
                            Copy = new Directory(std::move(*dir));
                            dirDest->addFile(Copy);
                            ((Directory *) Copy)->setParent(dirDest);
                            ((Directory *) Copy)->setParentsForChildren((Directory *) Copy);
                            used=true;

                        } else {
                            found2 = false;
                        }
                    } else {
                        if (!dirDest->checkF(dir->getName())) {
                             dirDest->addFile(Copy);
                            dir->removeFile(dir->getChildren().at(d));
                            used=true;
                        }
                        else{
                            found2=false;
                            delete(Copy);
                        }
                    }
                }
                else {
                    found2 = false;
                    cout << "No such file or directory" << endl;
                }
            }
        }
        else{ cout<<"Can't move directory"<<endl;}
    }
    if (!used){
        delete(Copy);
    }
    delete(destinationPathVector);
    delete(sourcePathVector);
}


string MvCommand::toString() {
    return "mv";
}

ErrorCommand::ErrorCommand(string args) : BaseCommand(args) {

}

string ErrorCommand::toString() {
    return "error";
}
void ErrorCommand::execute(FileSystem & fs){
    string s=this->getArgs().substr(0,this->getArgs().find(' '));
    cout<<s<<":"<<" Unknown command"<<endl;
}

VerboseCommand::VerboseCommand(string args) : BaseCommand(args) {

}

string VerboseCommand::toString() {
    return "verbose";
}

void VerboseCommand::execute(FileSystem &fs) {
    string s=this->getArgs().substr(this->getArgs().find(' ')+1);
    if(std::stoi(s)>=0 && std::stoi(s)<=3){
        verbose = static_cast<unsigned int>(std::stoi(s));

    }
    else{
        cout<<"Wrong verbose input"<<endl;
    }
}

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> &history):
        BaseCommand(args), history(history) {

}
string ExecCommand::toString() {
    return "exec";
}

void ExecCommand::execute(FileSystem &fs) {
    string s = this->getArgs().substr(this->getArgs().find(' ') + 1);
    size_t  i= static_cast<size_t>(std::stoi(s));
    if((i>=0) & (i<this->history.size())){
        BaseCommand* exec=this->history.at(i);
        exec->execute(fs);
    }
    else{
        cout<<"Command not found"<<endl;
    }
}