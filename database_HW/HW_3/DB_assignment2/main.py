
import pandas as pd
import pymysql
from sqlalchemy import create_engine

# 행: 100,000, 열: 40, 파일 크기: 27.9MB
df = pd.read_csv("./MovieCrawl.csv", encoding='utf-8')


# params
host='localhost'
user='chanha'
password='lee'
database='db_practice2'
unix_socket='/tmp/mysql.sock'
port = 3306
table_name = "movie"


# DB 접속 엔진 객체 생성
engine = create_engine(f'mysql+pymysql://{user}:{password}@{host}:{port}/{database}', encoding='utf-8')

# DB 테이블 명

# DB에 DataFrame 적재
df.to_sql(index = False,
          name = table_name,
          con = engine,
          if_exists = 'append',
          method = 'multi',
          chunksize = 10000)