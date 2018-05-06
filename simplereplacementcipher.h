#ifndef SIMPLEREPLACEMENTCIPHER_H
#define SIMPLEREPLACEMENTCIPHER_H

#include <QWidget>

class QTextEdit;
class QPushButton;
class OpenAndCodedText;
class QSpinBox;
class RelationTableModel;
class QTableView;

class SimpleReplacementCipher : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleReplacementCipher(QWidget *parent = nullptr);

signals:

public slots:
    void decode();

private:
    RelationTableModel *m_relationTableModel;
    QTableView *m_relationsTableView;
    OpenAndCodedText *m_openText;
    QPushButton *m_decode;
};

#endif // SIMPLEREPLACEMENTCIPHER_H
