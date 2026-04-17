#ifndef INPUTRECIEVER_H
#define INPUTRECIEVER_H

enum class InputAction;

class InputReciever
{

public:
    virtual void RecieveInput(InputAction) = 0;
    virtual bool InputRepeats() = 0;
    virtual ~InputReciever() = default;
};

#endif
