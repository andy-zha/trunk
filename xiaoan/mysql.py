#!usr/bin/python3

import config
import pymysql


def connect():
    db = pymysql.connect(
        config.HostCfg,
        config.UserCfg,
        config.PassCfg,
        config.DbCfg,
        config.PortCfg
    )
    return db


def exec_query(conn, sql):
    try:
        conn.cursor().execute(sql)
    except:
        return None
    return conn.cursor().fetchall()


def exec_sql(conn, sql):
    try:
        conn.cursor().execute(sql)
        conn.commit()
    except:
        conn.rollback()


def close(conn):
    conn.close()
