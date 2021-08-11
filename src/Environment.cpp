#include "Environment.h"
#include <iostream>
#include <typeinfo>

Environment::Environment():commandsHistory(),fs() {
}

void Environment::start() {
    int start=1;
    while (start==1){
        cout << fs.getWorkingDirectory().getAbsolutePath()<<">";
        std::string input;
        getline(std::cin,input);
        if((verbose==2)|(verbose==3)){
            cout<<input<<endl;
        }
        if(input.compare("exit")==0){
            start=0;
        }
        else if(input.compare("pwd")==0){
            PwdCommand* c = new PwdCommand(input);
            c->execute(this->fs);
            addToHistory(c);
        }
        else if(input.substr(0,input.find(' ')).compare("cd")==0){
            CdCommand* cd = new CdCommand(input);
            cd->execute(this->fs);
            addToHistory(cd);
        }
        else if((input.compare("ls")==0)|(input.substr(0,input.find(' ')).compare("ls")==0)){
            LsCommand* ls = new LsCommand(input);
            ls->execute(this->fs);
            addToHistory(ls);
        }
        else if(input.substr(0,input.find(' ')).compare("mkdir")==0){
            MkdirCommand* mkdir = new MkdirCommand(input);
            mkdir->execute(this->fs);
            addToHistory(mkdir);
        }
        else if(input.substr(0,input.find(' ')).compare("mkfile")==0){
            MkfileCommand* mk = new MkfileCommand(input);
            mk->execute(fs);
            addToHistory(mk);
        }
        else if(input.substr(0,input.find(' ')).compare("rename")==0){
            RenameCommand* rename = new RenameCommand(input);
            rename->execute(this->fs);
            addToHistory(rename);
        }
        else if(input.compare("history")==0){
            HistoryCommand* history = new HistoryCommand(input,getHistory());
            history->execute(this->fs);
            addToHistory(history);
        }
        else if(input.substr(0,input.find(' ')).compare(("rm"))==0){
            RmCommand*rm = new RmCommand(input);
            rm->execute(this->fs);
            addToHistory(rm);
        }
        else if(input.substr(0,input.find(' ')).compare("cp")==0){
            CpCommand* cp=new CpCommand(input);
            cp->execute(this->fs);
            addToHistory(cp);
        }
        else if(input.substr(0,input.find(' ')).compare("mv")==0){
            MvCommand* mv=new MvCommand(input);
            mv->execute(this->fs);
            addToHistory(mv);
        }
        else if(input.substr(0,input.find(' ')).compare("exec")==0 ){
            ExecCommand* exec=new ExecCommand(input,this->commandsHistory);
            exec->execute(fs);
            addToHistory(exec);
        }

        else if(input.substr(0,input.find(' ')).compare("verbose")==0 ){
            VerboseCommand* verbose=new VerboseCommand(input);
            verbose->execute(fs);
            addToHistory(verbose);
        }
        else{
            ErrorCommand* error=new ErrorCommand(input);
            error->execute(fs);
            addToHistory(error);
        }
    }
}


FileSystem& Environment::getFileSystem()  {
    return fs ;
}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

const vector<BaseCommand*>& Environment::getHistory() const {
    return commandsHistory;
}


Environment::~Environment() {
    if((verbose==1)|(verbose==3)){
        cout<<"Environment::~Environment()"<<endl;
    }
    while(commandsHistory.size()>0){
        delete((BaseCommand*)commandsHistory.at(0));
        commandsHistory.erase(commandsHistory.begin());
    }
}

Environment::Environment(const Environment& other):commandsHistory(other.commandsHistory),fs(other.fs) {
    if((verbose==1)|(verbose==3)){
        cout<<"Environment::Environment(const Environment& other)"<<endl;
    }
    this->fs=other.fs;
    removecommands(this->commandsHistory);
    CopyVector(other.commandsHistory);


}

Environment::Environment(Environment &&other):commandsHistory(other.commandsHistory),fs(other.fs) {
    if((verbose==1)|(verbose==3)){
        cout<<"Environment::Environment(Environment &&other)"<<endl;
    }
    delete(&fs);
    for(size_t i=0;i<other.commandsHistory.size();i++){
        this->commandsHistory.push_back((BaseCommand*)other.commandsHistory.at(i));
        other.commandsHistory.at(i)=nullptr;
    }
    other.commandsHistory.clear();
    other.commandsHistory.shrink_to_fit();
    fs=FileSystem(std::move(other.fs));
}


Environment &Environment::operator=(const Environment &other) {
    if((verbose==1)|(verbose==3)){
        cout<<"Environment &Environment::operator=(const Environment &other)"<<endl;
    }
    if(this!=&other){
        delete(&fs);
        fs=FileSystem(other.fs);
        removecommands(this->commandsHistory);
        CopyVector(other.commandsHistory);

    }
    return *this;
}








void Environment::removecommands(vector<BaseCommand *> s) {

    while(s.size()>0){
        delete(s.at(0));
        s.erase(s.begin());

    }
    s.clear();
    s.shrink_to_fit();
}

void Environment::CopyVector(vector<BaseCommand *> commandsHistory) {
    for(size_t i=0;i<commandsHistory.size();i++){

        if(typeid(commandsHistory.at(i))==typeid(PwdCommand)){
            this->commandsHistory.push_back(new PwdCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(CdCommand)){
            this->commandsHistory.push_back(new CdCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(LsCommand)){
            this->commandsHistory.push_back(new LsCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(MkdirCommand)){
            this->commandsHistory.push_back(new MkdirCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(MkfileCommand)){
            this->commandsHistory.push_back(new MkfileCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(CpCommand)){
            this->commandsHistory.push_back(new CpCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(MvCommand)){
            this->commandsHistory.push_back(new MvCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(RenameCommand)){
            this->commandsHistory.push_back(new RenameCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(RmCommand)){
            this->commandsHistory.push_back(new RmCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(VerboseCommand)){
            this->commandsHistory.push_back(new VerboseCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(ErrorCommand)){
            this->commandsHistory.push_back(new ErrorCommand(commandsHistory.at(i)->getArgs()));
        }
        else if(typeid(commandsHistory.at(i))==typeid(ExecCommand)){
            this->commandsHistory.push_back(new ExecCommand(commandsHistory.at(i)->getArgs(),commandsHistory));
        }
        else if(typeid(commandsHistory.at(i))==typeid(HistoryCommand)){
            this->commandsHistory.push_back(new HistoryCommand(commandsHistory.at(i)->getArgs(),commandsHistory));
        }
    }

}

Environment &Environment::operator=(Environment &&other) {
    if((verbose==1)|(verbose==3)){
        cout<<"Environment &Environment::operator=(Environment &&other)"<<endl;
    }
    if(this!=&other){
        delete(&fs);
        this->fs=FileSystem(std::move(other.fs));
        removecommands(this->commandsHistory);

        for(size_t i=0;i<other.commandsHistory.size();i++){
            this->commandsHistory.push_back((BaseCommand*)other.commandsHistory.at(i));
            other.commandsHistory.at(i)=nullptr;
        }
        other.commandsHistory.clear();
        other.commandsHistory.shrink_to_fit();
    }
    return *this;
}














