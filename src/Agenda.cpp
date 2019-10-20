#include <signal.h>
#include "AgendaUI.hpp"

using namespace std;
AgendaUI agenda;

void Stop(int signal){
    cout << endl <<  "Ctrl+c received! changes are saved." << endl;
    agenda.quitAgenda();
}

int main(){
    if(signal(SIGINT, Stop) == SIG_ERR)
    	cout << "unexpected signal received! changes are saved. " << endl;
    
    agenda.OperationLoop();
    return 0;
}