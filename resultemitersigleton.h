#ifndef RESULTEMITERSIGLETON_H
#define RESULTEMITERSIGLETON_H

#include <QObject>

class ResultEmiterSigleton : public QObject
	{
	Q_OBJECT

public:
	~ResultEmiterSigleton() { instance = NULL; }
	static ResultEmiterSigleton* getInstance();
	void sentResultRequest(const QStringList &list) const;

signals:
	void sentResult(const QStringList &list) const;
	void sentStatusMsg( const QString &msg, int timeout ) const;

private:
	ResultEmiterSigleton() {
		// private consructor
	}
	static ResultEmiterSigleton *instance;

	};

#endif // RESULTEMITERSIGLETON_H


//qRegisterMetaType<MyDataType>("MyDataType");
// Q_DECLARE_METATYPE(cv::Mat);
