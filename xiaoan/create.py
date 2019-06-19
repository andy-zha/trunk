#!usr/bin/python3

import mysql


if __name__ == '__main__':
    conn = mysql.connect()

    """创建根表"""
    sql = "CREATE TABLE IF NOT EXISTS root(answer TEXT NOT NULL, q_id int);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root(answer, q_id) SELECT  '我怎么了？', 1 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root WHERE q_id = 1);"
    mysql.exec_sql(conn,sql)

    """创建表root_1"""
    sql = "CREATE TABLE IF NOT EXISTS root_1(answer TEXT NOT NULL, q_id int);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1(answer, q_id) SELECT  '你好，有什么可以帮助你的吗？', 1 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1 WHERE q_id = 1);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1(answer, q_id) SELECT  '我在的，怎么了？', 2 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1 WHERE q_id = 2);"
    mysql.exec_sql(conn, sql)

    """创建表root_1_1"""
    sql = "CREATE TABLE IF NOT EXISTS root_1_1(answer TEXT NOT NULL, q_id int);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1_1(answer, q_id) SELECT  '你在说我很漂亮吗？谢谢', 1 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1_1 WHERE q_id = 1);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1_1(answer, q_id) SELECT  '我挺好的，谢谢你的关心', 2 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1_1 WHERE q_id = 2);"
    mysql.exec_sql(conn, sql)

    """创建表root_1_2"""
    sql = "CREATE TABLE IF NOT EXISTS root_1_2(answer TEXT NOT NULL, q_id int);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1_2(answer, q_id) SELECT  '你是在问我在干嘛吗？我在公司加班呢', 1 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1_2 WHERE q_id = 1);"
    mysql.exec_sql(conn,sql)

    """创建表root_1_1_1"""
    sql = "CREATE TABLE IF NOT EXISTS root_1_1_1(answer TEXT NOT NULL, q_id int);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1_1_1(answer, q_id) SELECT  '谢谢，我知道，哈哈哈。', 1 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1_1_1 WHERE q_id = 1);"
    mysql.exec_sql(conn, sql)

    """创建root_1_2_1"""
    sql = "CREATE TABLE IF NOT EXISTS root_1_2_1(answer TEXT NOT NULL, q_id int);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1_2_1(answer, q_id) SELECT  '我在公司加班呢，你呢？', 1 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1_2_1 WHERE q_id = 1);"
    mysql.exec_sql(conn, sql)

    """创建root_1_1_1_1"""
    sql = "CREATE TABLE IF NOT EXISTS root_1_1_1_1(answer TEXT NOT NULL, q_id int);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1_1_1_1(answer, q_id) SELECT  '谢谢，我知道，哈哈哈。', 1 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1_1_1_1 WHERE q_id = 1);"
    mysql.exec_sql(conn, sql)

    """创建root_1_2_1_1"""
    sql = "CREATE TABLE IF NOT EXISTS root_1_2_1_1(answer TEXT NOT NULL, q_id int);"
    mysql.exec_sql(conn, sql)
    sql = "INSERT INTO root_1_2_1_1(answer, q_id) SELECT  '我在公司加班啊，你呢？', 1 FROM DUAL WHERE NOT EXISTS (SELECT answer FROM root_1_2_1_1 WHERE q_id = 1);"
    mysql.exec_sql(conn, sql)


    mysql.close(conn)
