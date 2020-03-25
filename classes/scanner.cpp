#include "scanner.h"
#include <QString>
#include <QThread>
#include <QFile>
#include <QApplication>
#include <iostream>

typedef int(SCANNER_NEWTAGREADERFUNC)(int, int);
typedef int(SCANNER_CONNECTFUNC)(int);
typedef int(SCANNER_DOREADERACTIONFUNC)(int, int);
typedef int(SCANNER_GETREADERACTIONFUNC)(int);
typedef char* (SCANNER_GETCHIPIDFUNC)(int);
typedef char* (SCANNER_SENDCOMMANDFUNC)(int, int, char*);
Scanner *Scanner::instance=nullptr;
Scanner::Scanner(){
    instance=this;
//    if(!QFile::exists("tagReader.dll")){
//        handle = LoadLibrary(TEXT("C:\\Qt\\Projekte\\RRChipScanner\\src\\libs\\link\\rr\\tagReader.dll"));
//    }
}
bool Scanner::connect(){
    if (NULL == handle) {
       connected=false;
    } else {
        SCANNER_NEWTAGREADERFUNC *pNEWTAGREADERFUNC = (SCANNER_NEWTAGREADERFUNC *)::GetProcAddress(handle, "NewTagReader");
        if (pNEWTAGREADERFUNC) {
           tagReaderID = (*pNEWTAGREADERFUNC)(0xe6a, 0x350);
        }
        SCANNER_CONNECTFUNC *pCONNECTFUNC = (SCANNER_CONNECTFUNC *)::GetProcAddress(handle, "TRConnect");
        if (pCONNECTFUNC){
           if((*pCONNECTFUNC)(tagReaderID)==0){
               SCANNER_GETCHIPIDFUNC *pSERIALNUMBER = (SCANNER_GETCHIPIDFUNC*)::GetProcAddress(handle, "TRGetSerialNumber");
               if(pSERIALNUMBER){
                   readerSerialNumber=(*pSERIALNUMBER)(tagReaderID);
               }
               {
//                   char cmd[3]={2,(char)0x0E};
//                   sendCommand(0x03,cmd);
               }
               {
//                   std::hex
//                   char cmd[3]={5,(char)0x00};
//                   std::cout<<sendCommand(0x03,cmd)<<std::endl;
//                   std::cout<<cmd<<std::endl;
               }
               setReaderPower(18);
               connected=true;
           } else {
               connected=false;
           }
        }
    }
    return connected;
}
bool Scanner::disconnect(){
    if(handle!=NULL){
        SCANNER_CONNECTFUNC *pCONNECTFUNC = (SCANNER_CONNECTFUNC *)::GetProcAddress(handle, "TRDisconnect");
        connected=false;
        if (pCONNECTFUNC){
           if((*pCONNECTFUNC)(tagReaderID)==-1){
               connected=true;
           }
        }
    }
    return connected?false:true;
}
bool Scanner::isConnected(){
    return connected;
}
bool Scanner::setReaderPower(int value){
    if(tagReaderID>0){
        SCANNER_DOREADERACTIONFUNC *pDOREADERACTIONFUNC = (SCANNER_DOREADERACTIONFUNC *)::GetProcAddress(handle, "TRSetReaderPower");
        if (pDOREADERACTIONFUNC) {
           if((*pDOREADERACTIONFUNC)(tagReaderID,value)==0){curPower=value; return true;}
        }
    }
    return false;
}
char* Scanner::sendCommand(int cmd,char* data){
    char *response={};
    if(tagReaderID>0){
        SCANNER_SENDCOMMANDFUNC *pSENDCOMMANDFUNC = (SCANNER_SENDCOMMANDFUNC *)::GetProcAddress(handle, "TRSendCommand");
        if (pSENDCOMMANDFUNC) {
            response = (*pSENDCOMMANDFUNC)(tagReaderID,cmd,data);
        }
    }
    return response;
}
void Scanner::beebShort(){
    char cmd[2]={6};
    sendCommand(0x02,cmd);
}
void Scanner::greenLight(){
    char cmd[2]={8};
    sendCommand(0x02,cmd);
}
void Scanner::beebGreenLight(){
    char cmd[2]={2};
    sendCommand(0x02,cmd);
}
QString Scanner::readChipID(){
    QString st="";
    SCANNER_GETCHIPIDFUNC *pGETCHIPIDFUNC = (SCANNER_GETCHIPIDFUNC *)::GetProcAddress(handle, "TRGetChipID");
    if (pGETCHIPIDFUNC) {
        char *chipID={};
        chipID = (*pGETCHIPIDFUNC)(tagReaderID);
        st=QString::fromLocal8Bit(chipID);
        if(st.compare("Error: Receiving the devices ID failed")==0)st="";
//       if(strlen(chipID)>0)beebGreenLight();
    }
    return st;
}
void Scanner::run(){
    while(!QThread::currentThread()->isInterruptionRequested()){
        QString chipID =getInstance()->readChipID();
        if(chipID.length()>0&&chipID.length()<=10){
            emit getInstance()->newChipIDSignal(chipID);
        }
        QApplication::processEvents();
    }
}
QString Scanner::getSerialNumber(){
    return QString::fromLocal8Bit(readerSerialNumber);
}
