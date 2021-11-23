#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addingstudent.h"
#include "author.h"
#include "admin.h"
#include "addingparty.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    connect(ui->Authors, SIGNAL(triggered()), this, SLOT(on_Authors_clicked()));
    connect(ui->OpenFile, SIGNAL(triggered()), this, SLOT(on_OpenFile_clicked()));
    connect(ui->SaveFile, SIGNAL(triggered()), this, SLOT(on_SaveFile_clicked()));
    connect(ui->NewPassword, SIGNAL(triggered()), this, SLOT(on_NewPassword_clicked()));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./DataBase.db");

    if (db.open())
    {
        qDebug("open");
    }else
        qDebug("open error");

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE Журнал(ФИО TEXT, Дата_Рождения DATE, Группа TEXT, Фотография TEXT, Почта TEXT, Телефон_Родителей TEXT);");

    if( !query->exec())
            qDebug() << "Unable to create table:\n" << query->lastError();

    query->clear();
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    ui->DBnamespace->setModel(model);

    partyDB = QSqlDatabase::addDatabase("QSQLITE", "Second");
    partyDB.setDatabaseName("./PartyDatabase.db");

    if (partyDB.open())
    {
        qDebug("open");
    }else
        qDebug("open error");

    partyQuery = new QSqlQuery(partyDB);
    partyQuery->exec("CREATE TABLE Мероприятия(Название TEXT, Дата_Проведения DATE, Явка TEXT, Место TEXT, Группы TEXT);");

    if( !partyQuery->exec())
            qDebug() << "Unable to create table:\n" << partyQuery->lastError();

    partyQuery->clear();
    partyModel =  new QSqlTableModel(this, partyDB);
    partyModel->setTable("Мероприятия");
    partyModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    partyModel->select();

    ui->partyTableView->setModel(partyModel);

    updateComboBox();

    QString folderName = ui->ChooseParty->currentText();
    //qDebug()<<folderName;
    QString infoImagesPath = "D:/qt/CourseWork/source/Gallery/" + folderName + "/images.txt";
    //qDebug()<<infoImagesPath;

    QFile file(infoImagesPath);


    if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file.atEnd())
            {
                //читаем строку
                QString str = file.readLine();
                str.remove(str.length()-1, str.length());
                //Делим строку на слова разделенные пробелом
                images.push_back(str);
            }

        }
        else
        {
            qDebug()<< "can't open the file";
        }
    QPixmap pixmap(images[0]);
    ui->PartyImage->setPixmap(pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateComboBox()
{
    ui->ChooseParty->clear();
    int size = partyModel->rowCount();
    for (int i = 0; i<size; ++i)
    {
        ui->ChooseParty->addItem(partyModel->index(i,0).data().toString());
    }
}


//ВКЛАДКА АВТОР//
void MainWindow::on_Authors_clicked()
{
    Author* auth = new Author();
    auth->show();
}

//СОХРАНИТЬ БД//        ДОРАБОТАТЬ
void MainWindow::on_SaveFile_clicked()
{
    QString SaveDataBase = QFileDialog::getSaveFileName(0, "Сохранить базу данных", QDir::currentPath(), "Файлы баз данных (*.db)");
    QFile SaveDB(SaveDataBase);
    if( SaveDB.open( QIODevice::WriteOnly ) ) {
            QDataStream stream( &SaveDB );
            stream << QObject::trUtf8( "Проверка!" );
            SaveDB.close();
        }

}

//ОТКРЫТЬ БД//
void MainWindow::on_OpenFile_clicked()
{
    QString NewDataBaseName = QFileDialog::getOpenFileName(0, "Открыть базу данных", QDir::currentPath(), "Файлы баз данных (*.db)");
    db.setDatabaseName(NewDataBaseName);

    if (db.open())
    {
        qDebug("open");
    }else
        qDebug("open error");
}

//ИЗМЕНИТЬ ПАРОЛЬ//
void MainWindow::on_NewPassword_clicked()
{
    admin *newAdmin = new admin();
    newAdmin->show();
}

//ДОБАВИТЬ СТУДЕНТА//
void MainWindow::on_addStudent_clicked()
{
    //model->insertRow(model->rowCount());
    AddingStudent *newStudent = new AddingStudent();
    newStudent->show();

    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);
}

//УДАЛИТЬ СТУДЕНТА//
void MainWindow::on_deleteStudent_clicked()
{
    model->removeRow(row);
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);
}

//НАЖАТИЕ НА ТАБЛИЦУ//
void MainWindow::on_DBnamespace_clicked(const QModelIndex &index)
{
    row = index.row();

    QString FullName = model->index(row,0).data().toString();
    ui->FullNameLabel->setText(FullName);

    //Сколько лет
    QString AgeLevel = model->index(row,1).data().toString();
    ui->BirthdayTrueLabel->setText(AgeLevel);

    QDate currentDate = QDate::currentDate();
    QDate oldAge = QDate::fromString(AgeLevel, "dd.MM.yyyy");
    qint64 days = oldAge.daysTo(currentDate);
    days/=365;
    QString actualYears = QString::number(days); // actual conversion
    ui->AgeTrueLabel->setText(actualYears);
    //

    QString numberOfGroup = model->index(row,2).data().toString();
    ui->GroupTrueLabel->setText(numberOfGroup);

    QString ImageSrc = model->index(row, 3).data().toString();
    QPixmap pixmap(ImageSrc);
    ui->ImageLabel->setPixmap(pixmap);

    QString ParentsNumber = model->index(row,5).data().toString();
    ui->NumberTrueLabel->setText(ParentsNumber);

    QString Mail = model->index(row,4).data().toString();
    ui->MailTrueLabel->setText(Mail);

}

void MainWindow::on_editButton_clicked()
{

}


//ОБНОВИТЬ ТАБЛИЦУ//
void MainWindow::on_UpdateButton_clicked()
{
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);
}

void MainWindow::on_sortDBby_clicked()
{
    int index = ui->selectSortBy->currentIndex();
    if (index == 0)
    {
        model->sort(2, Qt::AscendingOrder);
    }else if (index == 1)
    {
        model->sort(0, Qt::AscendingOrder);
    }
}

void MainWindow::on_resetButton_clicked()
{
    ui->selectSortBy->setCurrentIndex(0);
    model->setTable("Журнал");
    model->select();
    ui->DBnamespace->setModel(model);
}

void MainWindow::on_partyButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_partyTableView_clicked(const QModelIndex &index)
{
    partyRow = index.row();

    QString PartyName = partyModel->index(partyRow,0).data().toString();
    ui->SetPartyName->setText(PartyName);

    QString PartyDate = partyModel->index(partyRow,1).data().toString();
    ui->SetPartyDate->setText(PartyDate);

    QString PartyAppearence = partyModel->index(partyRow,2).data().toString();
    ui->SetPartyAppearence->setText(PartyAppearence);

    QString PartyPlace = partyModel->index(partyRow,3).data().toString();
    ui->SetPartyPlace->setText(PartyPlace);

    QString PartyGroup = partyModel->index(partyRow,4).data().toString();
    ui->SetPartyGroup->setText(PartyGroup);

    updateComboBox();
}

//Функция удаления папки
int removeFolder(QDir & dir)
{
  int res = 0;
  //Получаем список каталогов
  QStringList lstDirs = dir.entryList(QDir::Dirs |
                  QDir::AllDirs |
                  QDir::NoDotAndDotDot);
  //Получаем список файлов
  QStringList lstFiles = dir.entryList(QDir::Files);

  //Удаляем файлы
  foreach (QString entry, lstFiles)
  {
   QString entryAbsPath = dir.absolutePath() + "/" + entry;
   QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
   QFile::remove(entryAbsPath);
  }

  //Для папок делаем рекурсивный вызов
  foreach (QString entry, lstDirs)
  {
   QString entryAbsPath = dir.absolutePath() + "/" + entry;
   QDir dr(entryAbsPath);
   removeFolder(dr);
  }

  //Удаляем обрабатываемую папку
  if (!QDir().rmdir(dir.absolutePath()))
  {
    res = 1;
  }
  return res;
}

void MainWindow::on_deletePartyButton_clicked()
{
    QString PartyName = partyModel->index(partyRow,0).data().toString();
    QDir dir;

    dir = "D:/qt/CourseWork/source/Gallery/" + PartyName + "/";
    removeFolder(dir);

    partyModel->removeRow(partyRow);

    partyModel =  new QSqlTableModel(this, partyDB);
    partyModel->setTable("Мероприятия");
    partyModel->select();
    partyModel->sort(1, Qt::DescendingOrder);
    ui->partyTableView->setModel(partyModel);
}

void MainWindow::on_addPartyButton_clicked()
{
    AddingParty *newParty = new AddingParty;
    newParty->show();
}

void MainWindow::on_updatePartyButton_clicked()
{
    partyModel =  new QSqlTableModel(this, partyDB);
    partyModel->setTable("Мероприятия");
    partyModel->select();
    partyModel->sort(1, Qt::DescendingOrder);
    ui->partyTableView->setModel(partyModel);

    updateComboBox();
}

void MainWindow::on_AddPhoto_clicked()
{

    QString folderName = ui->ChooseParty->currentText();
    QString infoImagesPath = "D:/qt/CourseWork/source/Gallery/" + folderName + "/images.txt";

    QFile file(infoImagesPath);

    QString PathToImage = QFileDialog::getOpenFileName(0, "Открыть изображение", QDir::currentPath(), "Файлы изображений (*.png *.jpg *.bmp)");

    if (file.open(QIODevice::Append)) {
        QTextStream out(&file);
        out<<PathToImage;
        out<<"\n";
    }

    file.close();
}
