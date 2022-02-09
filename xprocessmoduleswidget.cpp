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
#include "xprocessmoduleswidget.h"
#include "ui_xprocessmoduleswidget.h"

XProcessModulesWidget::XProcessModulesWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::XProcessModulesWidget)
{
    ui->setupUi(this);

    // mb TODO autorefresh

    g_nProcessId=0;
    g_pModel=0;
    g_pOldModel=0;
}

XProcessModulesWidget::~XProcessModulesWidget()
{
    delete ui;
}

void XProcessModulesWidget::setData(qint64 nProcessId)
{
    g_nProcessId=nProcessId;

    reload();
}

void XProcessModulesWidget::reload()
{
#ifdef QT_DEBUG
    qDebug("void XProcessModulesWidget::reload()");
#endif

    if(g_nProcessId)
    {
        g_pOldModel=g_pModel;

        XBinary::MODE modeAddress=XBinary::MODE_32;

        if(sizeof(void *)==8)
        {
            modeAddress=XBinary::MODE_64;
        }
        else
        {
            modeAddress=XBinary::MODE_32;
        }

        QList<XProcess::MODULE> listModules=XProcess::getModulesList(g_nProcessId);

        qint32 nNumberOfRecords=listModules.count();

        g_pModel=new QStandardItemModel(nNumberOfRecords,__HEADER_COLUMN_size);

        g_pModel->setHeaderData(HEADER_COLUMN_ADDRESS,Qt::Horizontal,tr("Address"));
        g_pModel->setHeaderData(HEADER_COLUMN_SIZE,Qt::Horizontal,tr("Size"));
        g_pModel->setHeaderData(HEADER_COLUMN_NAME,Qt::Horizontal,tr("Name"));
        g_pModel->setHeaderData(HEADER_COLUMN_FILENAME,Qt::Horizontal,tr("File name"));

        for(int i=0;i<nNumberOfRecords;i++)
        {
            QStandardItem *pItemAddress=new QStandardItem;
            pItemAddress->setText(XBinary::valueToHex(modeAddress,listModules.at(i).nAddress));
            pItemAddress->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_ADDRESS,pItemAddress);

            QStandardItem *pItemSize=new QStandardItem;
            pItemSize->setText(XBinary::valueToHex(XBinary::MODE_32,listModules.at(i).nSize));
            pItemSize->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_SIZE,pItemSize);

            QStandardItem *pItemName=new QStandardItem;
            pItemName->setText(listModules.at(i).sName);
            pItemName->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_NAME,pItemName);

            QStandardItem *pItemFileName=new QStandardItem;
            pItemFileName->setText(listModules.at(i).sFileName);
            pItemFileName->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_FILENAME,pItemFileName);
        }

        ui->tableViewModules->setModel(g_pModel);

        #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            QFuture<void> future=QtConcurrent::run(&XProcessModulesWidget::deleteOldModel,this);
        #else
            QFuture<void> future=QtConcurrent::run(this,&XProcessModulesWidget::deleteOldModel);
        #endif
    }
}

void XProcessModulesWidget::deleteOldModel()
{
    if(g_pOldModel)
    {
        delete g_pOldModel;

        g_pOldModel=0;
    }
}

void XProcessModulesWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void XProcessModulesWidget::on_pushButtonSave_clicked()
{
    if(g_pModel)
    {
        XShortcutsWidget::saveModel(g_pModel,QString("%1.txt").arg(tr("Modules")));
    }
}

void XProcessModulesWidget::on_pushButtonReload_clicked()
{
    reload();
}
