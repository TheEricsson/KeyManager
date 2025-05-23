#ifndef INITDB_H
#define INITDB_H

#include <QDate>
#include <QSqlError>
#include <QSqlQuery>

// my stuff

enum keyCategory
{
    Technical,
    Central,
    Appartment,
    Facility,
    Collection
};

enum keyStatus
{
    InHouse,
    TemporaryOut,
    PermanentOut,
    Divorced,
    Lost,
};

// the 'final' adding function with all parameters for a key
void addKey (QSqlQuery &q, const QVariant &adress, int count, keyCategory cat, const QString &description, keyStatus state, QVariant issueDate, QVariant &deadline)
{
    q.addBindValue(adress);
    q.addBindValue(count);
    q.addBindValue(cat);
    q.addBindValue(description);
    q.addBindValue(state);
    q.addBindValue(issueDate);
    q.addBindValue(deadline);
    q.exec();
}

QVariant addAdress (QSqlQuery &q, const QString &street, const QString &houseNumber, const QString &areaCode, const QString &city)
{
    q.addBindValue(street);
    q.addBindValue(houseNumber);
    q.addBindValue(areaCode);
    q.addBindValue(city);
    q.exec();
    return q.lastInsertId();
}

QVariant addstate (QSqlQuery &q, keyStatus state)
{
    q.addBindValue(state);
    q.exec();
    return q.lastInsertId();
}

QVariant addDeadline (QSqlQuery &q, const QString &deadline)
{
    q.addBindValue(deadline);
    q.exec();
    return q.lastInsertId();
}

QVariant addIssueDate (QSqlQuery &q, const QString &issueDate)
{
    q.addBindValue(issueDate);
    q.exec();
    return q.lastInsertId();
}

QVariant addRecipient (QSqlQuery &q, const QString &recipient)
{
    q.addBindValue(recipient);
    q.exec();
    return q.lastInsertId();
}

QVariant addCompany (QSqlQuery &q, const QString &company)
{
    q.addBindValue(company);
    q.exec();
    return q.lastInsertId();
}

QVariant addSignature(QSqlQuery &q, const QString &signature)
{
    q.addBindValue(signature);
    q.exec();
    return q.lastInsertId();
}

// example stuff

void addBook(QSqlQuery &q, const QString &title, int year, const QVariant &authorId,
             const QVariant &genreId, int rating)
{
    q.addBindValue(title);
    q.addBindValue(year);
    q.addBindValue(authorId);
    q.addBindValue(genreId);
    q.addBindValue(rating);
    q.exec();
}

QVariant addGenre(QSqlQuery &q, const QString &name)
{
    q.addBindValue(name);
    q.exec();
    return q.lastInsertId();
}

QVariant addAuthor(QSqlQuery &q, const QString &name, QDate birthdate)
{
    q.addBindValue(name);
    q.addBindValue(birthdate);
    q.exec();
    return q.lastInsertId();
}

const auto BOOKS_SQL = QLatin1String(R"(
    create table books(id integer primary key, title varchar, author integer,
                       genre integer, year integer, rating integer)
    )");

const auto AUTHORS_SQL =  QLatin1String(R"(
    create table authors(id integer primary key, name varchar, birthdate date)
    )");

const auto GENRES_SQL = QLatin1String(R"(
    create table genres(id integer primary key, name varchar)
    )");

const auto INSERT_AUTHOR_SQL = QLatin1String(R"(
    insert into authors(name, birthdate) values(?, ?)
    )");

const auto INSERT_BOOK_SQL = QLatin1String(R"(
    insert into books(title, year, author, genre, rating)
                      values(?, ?, ?, ?, ?)
    )");

const auto INSERT_GENRE_SQL = QLatin1String(R"(
    insert into genres(name) values(?)
    )");

QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if (tables.contains("books", Qt::CaseInsensitive)
        && tables.contains("authors", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;
    if (!q.exec(BOOKS_SQL))
        return q.lastError();
    if (!q.exec(AUTHORS_SQL))
        return q.lastError();
    if (!q.exec(GENRES_SQL))
        return q.lastError();

    if (!q.prepare(INSERT_AUTHOR_SQL))
        return q.lastError();
    QVariant asimovId = addAuthor(q, QLatin1String("Isaac Asimov"), QDate(1920, 2, 1));
    QVariant greeneId = addAuthor(q, QLatin1String("Graham Greene"), QDate(1904, 10, 2));
    QVariant pratchettId = addAuthor(q, QLatin1String("Terry Pratchett"), QDate(1948, 4, 28));

    if (!q.prepare(INSERT_GENRE_SQL))
        return q.lastError();
    QVariant sfiction = addGenre(q, QLatin1String("Science Fiction"));
    QVariant fiction = addGenre(q, QLatin1String("Fiction"));
    QVariant fantasy = addGenre(q, QLatin1String("Fantasy"));

    if (!q.prepare(INSERT_BOOK_SQL))
        return q.lastError();
    addBook(q, QLatin1String("Foundation"), 1951, asimovId, sfiction, 3);
    addBook(q, QLatin1String("Foundation and Empire"), 1952, asimovId, sfiction, 4);
    addBook(q, QLatin1String("Second Foundation"), 1953, asimovId, sfiction, 3);
    addBook(q, QLatin1String("Foundation's Edge"), 1982, asimovId, sfiction, 3);
    addBook(q, QLatin1String("Foundation and Earth"), 1986, asimovId, sfiction, 4);
    addBook(q, QLatin1String("Prelude to Foundation"), 1988, asimovId, sfiction, 3);
    addBook(q, QLatin1String("Forward the Foundation"), 1993, asimovId, sfiction, 3);
    addBook(q, QLatin1String("The Power and the Glory"), 1940, greeneId, fiction, 4);
    addBook(q, QLatin1String("The Third Man"), 1950, greeneId, fiction, 5);
    addBook(q, QLatin1String("Our Man in Havana"), 1958, greeneId, fiction, 4);
    addBook(q, QLatin1String("Guards! Guards!"), 1989, pratchettId, fantasy, 3);
    addBook(q, QLatin1String("Night Watch"), 2002, pratchettId, fantasy, 3);
    addBook(q, QLatin1String("Going Postal"), 2004, pratchettId, fantasy, 3);

    return QSqlError();
}

#endif // INITDB_H
