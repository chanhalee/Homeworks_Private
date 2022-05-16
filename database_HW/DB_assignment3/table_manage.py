import requests
from bs4 import BeautifulSoup
import pymysql


def open_db():
    conn = pymysql.connect(host='localhost', user='chanha', password='lee', db='db_practice2',
                           unix_socket='/tmp/mysql.sock')

    curs = conn.cursor(pymysql.cursors.DictCursor)

    return conn, curs


def close_db(curs, conn):
    curs.close()
    conn.close()


def execute(sql):
    conn, curs = open_db()

    curs.execute(sql)

    close_db(curs, conn)

createTable = """create table movie(
	id int auto_increment primary key,
    title varchar(25) not null,
    movie_rate varchar(10) default null,
    netizen_rate double default null,
    netizen_count integer default 0,
    journalist_score double default null,
    journalist_count integer default 0,
    scope varchar(25),
    playing_time integer default 0,
    opening_date date default null,
    director varchar(25),
    image varchar(250),
    enter_date datetime default now() on update now()
    );"""
	
dropTable = """ drop table movie;
"""

execute(dropTable)
execute(createTable)