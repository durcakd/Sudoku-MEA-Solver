#ifndef RESULTEMITERSIGLETON_H
#define RESULTEMITERSIGLETON_H

#include <QObject>

class ResultEmiterSigleton : public QObject
	{
	Q_OBJECT

public:
	~ResultEmiterSigleton() { instance = NULL; }
	static ResultEmiterSigleton* getInstance();


private:
	ResultEmiterSigleton() {
		// private consructor
	}
	static ResultEmiterSigleton *instance;

	};

#endif // RESULTEMITERSIGLETON_H


//qRegisterMetaType<MyDataType>("MyDataType");
