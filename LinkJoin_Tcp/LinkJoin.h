#ifndef LINKJOIN_H
#define LINKJOIN_H

#pragma execution_character_set("utf-8")	// utf-8

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QTableWidget>
#include <cmath>
#include <QPushButton>
#include <QDir>

#if 1
#define LJ_DEBUG(...)		qDebug()<<__FILE__<<"("<<__LINE__<<"):"<<__FUNCTION__<<":"<<##__VA_ARGS__
	#define LJ_BOX(t,c)			QMessageBox::warning(nullptr,t,c)
	#define LJ_ASSERT(b)		Q_ASSERT(b)
#else
	#define LJ_DEBUG(...)	((void)0)
	#define LJ_BOX(t,c)		((void)0)
	#define LJ_ASSERT(b)	((void)0)
#endif // LJ_DEBUG

#define LJ_INF 0x3F3F3F3F

class LinkJoin {
public:
	LinkJoin() {
	}
	~LinkJoin() {
	}

	static QList<int>	QStringToQListInt(const QString &data, const QStringList &separators) {	// QString ת QList<int>
		QList<int> list;
		QString re_str = "[]";
		for (const auto &i : separators) {
			re_str.insert(1, i);
		}
		QStringList tmp_list = data.split(QRegExp(re_str), QString::SkipEmptyParts);
		for (const auto &i : tmp_list) {
			list.append(i.toInt());
		}
		return list;
	}

	static QList<float>	QStringToQListFloat(const QString &data, const QStringList &separators) {	// QString ת QList<float>
		QList<float> list;
		QString re_str = "[]";
		for (const auto &i : separators) {
			re_str.insert(1, i);
		}
		QStringList tmp_list = data.split(QRegExp(re_str), QString::SkipEmptyParts);
		for (const auto &i : tmp_list) {
			list.append(i.toFloat());
		}
		return list;
	}

	template<class T>	// ģ�岻�ܷ� ��
	static QString		QListNumberToQString(const QList<T>& datas, const QString& separator = ",") {	// QList<T> ת QString TΪ����
		QString ret_str;
		for (const auto &i : datas) {
			ret_str.append(QString::number(i) + separator);
		}
		ret_str.chop(separator.size());	// ɾ�������һ���ָ���
		return ret_str;
	}

	template<class T>	// ģ��
	static void DeleteP(T* p) {	// �ͷ�ָ��
		if (Q_NULLPTR != p) {	// �жϿ�
			delete p;
			p = Q_NULLPTR;	// �ǵ��ÿ�
		}
	}

	static void IndexCheck(int& index, const int& len) {	// Խ����
		if (0 >= len) {
			index = 0;
		} else {
			while (index < 0) {
				index += len;
			}
			while (index >= len) {
				index -= len;
			}
		}
	}

	static void TableWidgetSwapRow(QTableWidget *table, int row0, int row1) {
		int row_count = table->rowCount();
		int col_count = table->columnCount();
		if (Q_NULLPTR == table
			|| row0 == row1
			|| (row0 < 0 || row1 < 0)
			|| (row0 >= row_count || row1 >= row_count)) {
			return;
		}
		QTableWidgetItem *item;
		QWidget *widget;
		for (int i = 0; i < col_count; ++i) {
			item = table->takeItem(row0, i);
			table->setItem(row0, i, table->takeItem(row1, i));
			table->setItem(row1, i, item);
		}
	}

	static void TableWidgetSelectRow(QTableWidget *table, int row) {
		table->selectRow(row);
		table->setFocus();
	}

	static QString GetTime() {
		return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	}

	static bool Ask(const QString &title, const QString &text, 
		const QString &button_true = QObject::tr("ȷ��"), 
		const QString &button_false = QObject::tr("ȡ��"), 
		const QMessageBox::Icon &icon = QMessageBox::Warning) {
		QMessageBox mess(icon, title, text);
		mess.addButton(button_true, QMessageBox::YesRole);
		QPushButton *btn = mess.addButton(button_false, QMessageBox::NoRole);
		mess.exec();
		if (qobject_cast<QPushButton*>(mess.clickedButton()) == btn) {	// ȡ��
			delete btn; btn = Q_NULLPTR;
			return false;
		}
		delete btn; btn = Q_NULLPTR;
		return true;
	}

	static void MakeDirIfNotExists(const QString &path) {
		QDir dir;
		if (!dir.exists(path)) {	// ����ļ��в�����
			dir.mkpath(path);		//   �򴴽��ļ���
		}
	}

	static QString ExcelHCalc(const int &n) {
		QString h;
		int _n = n;
		while (_n > 0) {
			h.prepend(static_cast<char>((_n % 26) + 'A' - 1));
			_n /= 26;
		}
		return h;
	}
};

#endif	// LINKJOIN_H