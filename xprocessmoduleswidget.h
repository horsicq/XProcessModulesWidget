/* Copyright (c) 2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef XPROCESSMODULESWIDGET_H
#define XPROCESSMODULESWIDGET_H

#include <QMenu>
#include <QWidget>
#include <QStandardItemModel>
#include <QFuture>
#include <QtConcurrent>
#include "dialogdumpprocess.h"
#include "xprocess.h"
#include "xshortcutswidget.h"

namespace Ui {
class XProcessModulesWidget;
}

class XProcessModulesWidget : public XShortcutsWidget
{
    Q_OBJECT

    enum HEADER_COLUMN
    {
        HEADER_COLUMN_ADDRESS=0,
        HEADER_COLUMN_SIZE,
        HEADER_COLUMN_NAME,
        HEADER_COLUMN_FILENAME,
        __HEADER_COLUMN_size
    };

    enum SC
    {
        SC_DUMPTOFILE=0,
        __SC_SIZE
    };

    enum USERROLE
    {
        USERROLE_SIZE=0,
        USERROLE_ADDRESS
    };

public:
    explicit XProcessModulesWidget(QWidget *pParent=nullptr);
    ~XProcessModulesWidget();

    void setData(qint64 nProcessId,bool bReload=true);
    void reload();

private:
    void deleteOldModel();

protected:
    virtual void registerShortcuts(bool bState);

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonReload_clicked();
    void on_tableViewModules_customContextMenuRequested(const QPoint &pos);

protected slots:
    void _dumpToFileSlot();

private:
    Ui::XProcessModulesWidget *ui;
    qint64 g_nProcessId;
    QStandardItemModel *g_pModel;
    QStandardItemModel *g_pOldModel;
    QShortcut *shortCuts[__SC_SIZE];
};

#endif // XPROCESSMODULESWIDGET_H
