//
// Created by noy on ٢١‏/١٢‏/٢٠١٩.
//

#include <string>
#include "VarCommand.h"
class BlockCommand;

using namespace std;

class CreateVarCommand : public Command {
private:
	BlockCommand *block;
public:
	CreateVarCommand(BlockCommand *_block) : block(_block) {};
	int execute(std::vector<std::string> commands, int pos);
};

