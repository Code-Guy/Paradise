#ifndef BATCHCREATEDIALOG_H
#define BATCHCREATEDIALOG_H

#include <QDialog>
#include "global.h"

#define BATCH_CREATE_DIALOG_WIDTH 526
#define BATCH_CREATE_DIALOG_HEIGHT 277

namespace Ui {
class BatchCreateDialog;
}

class BatchCreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatchCreateDialog(QWidget *parent = 0);
    ~BatchCreateDialog();

	void Init(QString curTexturePath);

	void SetOverlapTest(bool ret);

signals:
	void OverlapTest(glm::ivec3 startPoint, glm::ivec3 endPoint);
	void AddBatchCellToBlock(glm::ivec3 startPoint, glm::ivec3 endPoint);

protected:
	virtual void keyPressEvent(QKeyEvent *evt);

private slots:
	void VarifyBtnClicked();
	void AddBtnClicked();
	void CancelBtnClicked();

private:
    Ui::BatchCreateDialog *ui;

	glm::ivec3 startPoint;
	glm::ivec3 endPoint;
};

#endif // BATCHCREATEDIALOG_H
