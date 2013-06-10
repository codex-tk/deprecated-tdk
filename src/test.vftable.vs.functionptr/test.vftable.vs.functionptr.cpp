// test.vftable.vs.functionptr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class command {
public:
	command( void ){
	}
	virtual ~command (void){
	}

	virtual void operator()( void ) = 0;
};


class command1 : public command {
public:
	command1( void ){
		memcpy( name , "command1" , 8 );
	}
	virtual ~command1 (void){
	}

	virtual void operator()( void ) {
		printf( "command1\r\n" );
	}
	char name[256];
};

class command2 : public command {
public:
	command2( void ){
		memcpy( name , "command2" , 8 );
	}
	virtual ~command2 (void){
	}

	virtual void operator()( void ) {
		printf( "command2\r\n" );
	}
	char name[256];
};

class command3 : public command {
public:
	command3( void ){
		memcpy( name , "command3" , 8 );
	}
	virtual ~command3 (void){
	}

	virtual void operator()( void ) {
		printf( "command3\r\n" );
	}
	char name[256];
};

void command_function1 (void* ){
	printf( "command3\r\n" );
}

void command_function2 (void* ){
	printf( "command3\r\n" );
}

void command_function3 (void* ){
	printf( "command3\r\n" );
}

typedef void (* cmd_function)(void*);


class call_wrapper {
public:
	cmd_function fn;

	void call( void ) {
		fn( this );
	}
};



int _tmain(int argc, _TCHAR* argv[])
{
	command* c  = nullptr;

	int id = (int)(&c) % 3;

	switch (id)
	{
	case 0:
		c = new command1();
		break;
	case 1:
		c = new command2();
		break;
	case 2:
		c = new command3();
		break;
	default:
		break;
	}

	(*c)();

	call_wrapper cmd;


	switch (id)
	{
	case 0:
		cmd.fn = &command_function1;
		break;
	case 1:
		cmd.fn = &command_function2;
		break;
	case 2:
		cmd.fn = &command_function3;
		break;
	default:
		break;
	}

	cmd.call();

	return 0;
}

