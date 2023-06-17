#include "texedit.h"
#include "ui_texedit.h"

TexEdit::TexEdit(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TexEdit)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->u_Text);

    // connect the ui buttons with their functions
    connect(ui->actionNew, &QAction::triggered, this, &TexEdit::newFile);
    connect(ui->actionOpen, &QAction::triggered, this, &TexEdit::open);
    connect(ui->actionSave, &QAction::triggered, this, &TexEdit::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &TexEdit::saveAs);
    connect(ui->actionQuit_2, &QAction::triggered, this, &TexEdit::quit);


    connect(ui->actionUndo, &QAction::triggered, this, &TexEdit::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &TexEdit::redo);
    connect(ui->actionFont, &QAction::triggered, this, &TexEdit::changeFont);
    connect(ui->actionAbout, &QAction::triggered, this, &TexEdit::about);

    // shortcuts
    ui->actionNew->setShortcut(QKeySequence("Ctrl+n"));
    ui->actionOpen->setShortcut(QKeySequence("Ctrl+o"));
    ui->actionSave->setShortcut(QKeySequence("Ctrl+s"));
    ui->actionSave_As->setShortcut(QKeySequence("Ctrl+Shift+s"));
    ui->actionUndo->setShortcut(QKeySequence("Ctrl+z"));
    ui->actionRedo->setShortcut(QKeySequence("Ctrl+y"));
}

TexEdit::~TexEdit()
{
    delete ui;
}


void TexEdit::newFile() {

    m_CurrentFileName.clear();
    ui->u_Text->setText(QString());
}

void TexEdit::save()
{
    QString fileName;
    // If we don't have a filename from before, get one.
    if (m_CurrentFileName.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        m_CurrentFileName = fileName;
    }
    else {
        fileName = m_CurrentFileName;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->u_Text->toPlainText();
    out << text;
    file.close();
}

void TexEdit::quit()
{
    QCoreApplication::quit();
}

void TexEdit::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    m_CurrentFileName = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->u_Text->setText(text);
    file.close();
}

void TexEdit::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    m_CurrentFileName = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->u_Text->toPlainText();
    out << text;
    file.close();
}

void TexEdit::undo()
{
    ui->u_Text->undo();
}

void TexEdit::redo()
{
    ui->u_Text->redo();
}

void TexEdit::changeFont()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->u_Text->setFont(font);
}

void TexEdit::about()
{
    QMessageBox::about(this, "About",
                       "This project was created for an academic project by "
                       "The Polymorphic Four: "
                       "Aditya Sharma, "
                       "Amitansu Raj Neti, "
                       "Anant  Rajput, "
                       "Satyarth Singh"
                       );
}

void TexEdit::closeEvent(QCloseEvent* theEvent)
{
    theEvent->ignore();
    if (m_CurrentFileName == "")
    {
        if (QMessageBox::Yes == QMessageBox::question(this, "Not Saved", "Are you sure you want to close?", QMessageBox::Yes | QMessageBox::No))
        {
            theEvent->accept();
        }
    }
    else
    {
        theEvent->accept();
    }
}
