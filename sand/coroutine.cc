#include <iostream> 
#include <iomanip>
#include <string>


_Coroutine DummyCo{

    int a;
    int b;

    void main(){
        std::cout << "Entering co-main\n";

        for(;;){

            if(b == 10){
                _Resume Err{} _At resumer();
                suspend();
            }
            
        }
    }

    void caller(int a, int b){
        if(a == 5){
            std::cout << "a is 5 -> resuming\n";
            resume();
        }
    }

public:
    _Exception Err{};
}




int main(int argc, char* argv[]){

    DummyCo rtn;
    try{
        _Enable{
            rtn.caller(5, 10);
        }
    }
    catch(DummyCo::Err& e){
        std::cout << "Caught an e\n";
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}