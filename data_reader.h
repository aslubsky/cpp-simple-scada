#ifndef DATAREADER_H
#define DATAREADER_H

class DataReader
{
public:
	DataReader();
	~DataReader();
	virtual void connect();
	virtual double read();
};

#endif // DATAREADER_H
