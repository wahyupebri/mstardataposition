#include "model.h"
#include "pch.h"

#include <string.h>
#include <iostream>
#include <stdio.h>


const long long INTERVALINMILSEC = 20000;//run query for every 20 secs
const char* PREFIX[] = { "CB", "KB", "HB" };
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System;
using namespace std;
int inputData(void* data);
int testModel() {
    printf("model.h\n");
    return 0;
}
SqlConnection^ initSqlConnect() {
    try{
        String^ connection = "Data Source=10.4.17.13;User ID=mshist;Password=mshistTutu;Connect Timeout=30;Encrypt=False;Trust Server Certificate=False;Application Intent=ReadWrite;Multi Subnet Failover=False";
        System::Console::WriteLine(connection);
        SqlConnection sqlConn(connection);
        sqlConn.Open();
        printf("Test success\n");
        return %sqlConn;
    }
    catch (Exception^ e) {
        printf("Error connecting to Database\n");
        System::Console::WriteLine(e->ToString());
    }
    return nullptr;
}

int getResultQuery(long long* waitformillsecs)
{
    long long startTime = getMillisec();
    long long endTime = 0;
    dataholder dholder;
    String^ query = "SELECT TOP (1000) P.[MACHINE_OID], J1.[NAME] as MACHINE, [CLASS_NAME], DATEADD(HOUR, 8, [LAST_POSITION_REPORT_DATE_UTC]) AS LAST_POSITION_REPORT, [X], [Y], [Z], [POSITION_ACCURACY], [FIELD_BAD_GPS], [SPEED], [HEADING] FROM [mspitmodel_rep].[dbo].[MACHINE_IN_PIT] P INNER JOIN [msmodel_rep].[dbo].[MACHINE] J1 ON P.[MACHINE_OID] = J1.[MACHINE_OID] ORDER BY [LAST_POSITION_REPORT_DATE_UTC] DESC;";
    
    try {
        String^ connection = "Data Source=10.4.17.13;Initial Catalog=mspitmodel_rep;User ID=mshist;Password=mshistTutu";
        SqlConnection sqlConn(connection);
        sqlConn.Open();
        SqlCommand command(query, %sqlConn);
        SqlDataReader^ reader = command.ExecuteReader();
        if (!reader->HasRows) {
            printf("[WARNING] Data empty\n");
            return 1;
        }
        while (reader->Read()) {
            char temp[50];
            String^ result = reader->GetValue(0)->ToString();
            char* cptr = convertStrToChar(result);
            dholder.machine_oid = strtol(cptr,nullptr,0);
            
            free(cptr);
            result = reader->GetValue(1)->ToString();
            cptr = convertStrToChar(result);
            strcpy(dholder.machine,cptr);
            //printf("Hasil %s\n", cptr);
            free(cptr);
            result = reader->GetValue(2)->ToString();
            cptr = convertStrToChar(result);
            strcpy(dholder.class_name, cptr);
            //printf("Hasil %s\n", cptr);
            free(cptr);

            result = reader->GetValue(3)->ToString();
            cptr = convertStrToChar(result);
            strcpy(dholder.last_pr, cptr);
            free(cptr);
            result = reader->GetValue(4)->ToString();
            cptr = convertStrToChar(result);
            convertCommaToPoint(cptr, temp);
            dholder.x = strtod(temp, nullptr);
            //printf("Hasil %lf\n", dholder.x);
            free(cptr);
            result = reader->GetValue(5)->ToString();
            cptr = convertStrToChar(result);
            convertCommaToPoint(cptr, temp);
            dholder.y = strtod(temp, nullptr);
            free(cptr);
            result = reader->GetValue(6)->ToString();
            cptr = convertStrToChar(result);
            convertCommaToPoint(cptr, temp);
            dholder.z = strtod(temp, nullptr);
            free(cptr);
            result = reader->GetValue(7)->ToString();
            cptr = convertStrToChar(result);
            convertCommaToPoint(cptr, temp);
            dholder.accuracy = strtod(temp, nullptr);
            //printf("Hasil %lf\n", dholder.accuracy);
            //printf("Hasil str %s\n", temp);
            free(cptr);
            result = reader->GetValue(8)->ToString();
            cptr = convertStrToChar(result);
            convertCommaToPoint(cptr, temp);
            dholder.bad_gps = strtod(temp, nullptr);
            //printf("Hasil %d\n", dholder.bad_gps);
            //printf("Hasil str %s\n", temp);
            free(cptr);
            result = reader->GetValue(9)->ToString();
            cptr = convertStrToChar(result);
            convertCommaToPoint(cptr, temp);
            dholder.speed = strtod(temp, nullptr);
            //printf("Hasil %lf\n", dholder.speed);
            //printf("Hasil str %s\n", temp);
            free(cptr);
            result = reader->GetValue(10)->ToString();
            cptr = convertStrToChar(result);
            convertCommaToPoint(cptr, temp);
            dholder.heading = strtod(temp, nullptr);
           //printf("Hasil %lf\n", dholder.heading);
            //printf("Hasil str %s\n", temp);
            free(cptr);
            //save to db
            if (!inputData(&dholder)){
                printf("[ERROR] Failed to insert new row\n");
                return 1;
            }
        }
        
        
        reader->Close();
        sqlConn.Close();

    }
    catch (Exception^ e) {
        System::Console::WriteLine(query);
        System::Console::WriteLine(e->ToString());
    }
    finally {
        endTime = getMillisec();
        printf("[INFO] Query done %ld ms\n", (endTime - startTime));
    }

    *waitformillsecs = INTERVALINMILSEC-(endTime - startTime);
    printf("Hasil func wait %d\n", *waitformillsecs);
    return 0;
}
int inputDataMySQL(void* data) {
    dataholder* pholder = (dataholder*)data;
    char* pname = pholder->machine;
    pname = pname + 2;
    MYSQL* conn;
    MYSQL_RES* result;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    printf("Insert to DB %s %f %f %s\n", pname,pholder->x,pholder->y,pholder->last_pr);
    if (conn == NULL) {
        fprintf(stderr, "[ERROR] mysql_init() failed\n");
        return 0;
    }
    if (mysql_real_connect(conn, "localhost", "root", "", "tracking_digger", 0, NULL, 0) == NULL) {
        fprintf(stderr, "[ERROR] mysql_real_connect() failed\n");
        mysql_close(conn);
        return 0;
    }

    char query[300] = "INSERT INTO position (pos_id,pos_timestamp, pos_machine, pos_x, pos_y) VALUES (NULL, ";
    char convert[50];
    strcat(query, "\'");
    printf("INFO: timestamp %s\n", pholder->last_pr);
    convertDataTimeFormat("%m/%d/%Y %I:%M:%S %p", "%Y-%m-%d %H:%M:%S", pholder->last_pr, convert);
    printf("INFO: timestamp convert %s\n", convert);
    strcat(query, convert);
    strcat(query, "\', ");
    strcat(query, pname);
    strcat(query, ", ");
    convertDoubleToCharArray(&pholder->x,convert);
    strcat(query, convert);
    strcat(query, ", ");
    convertDoubleToCharArray(&pholder->y, convert);
    strcat(query, convert);
    strcat(query, ");");
    //query
    fprintf(stdout, "[INFO] Query MySQL, %s\n", query);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "[ERROR] Query failed, %s\n", mysql_error(conn));
        mysql_close(conn);
        return 0;
    }
    
    
    mysql_close(conn);
    return 1;
}
int inputData(void* data) {
    dataholder* dholder = (dataholder*)data;
    String^ connection = "Data Source=10.4.17.13;Initial Catalog=DB_OnboardFMS;User ID=ADT_INT_WEBAPPS_PO;Password=buMA1233498AJDKAJ$%$##";

    if(dholder->machine[1]=='B'){
        inputDataMySQL(data);
    }
    /*wchar_t* machine = getWC(dholder->machine);
    wchar_t* class_name = getWC(dholder->class_name);
    wchar_t* activity = getWC(dholder->activity);
    wchar_t* last_pr = getWC(dholder->last_pr);
    char temp[20];
    convertDoubleCommaToCharPoint(&dholder->x,temp);
    wchar_t* x = getWC(temp);
    convertDoubleCommaToCharPoint(&dholder->y, temp);
    wchar_t* y = getWC(temp);
    convertDoubleCommaToCharPoint(&dholder->z, temp);
    wchar_t* z = getWC(temp);
    convertDoubleCommaToCharPoint(&dholder->accuracy, temp);
    wchar_t* accuracy = getWC(temp);
    convertDoubleCommaToCharPoint(&dholder->speed, temp);
    wchar_t* speed = getWC(temp);
    convertDoubleCommaToCharPoint(&dholder->heading, temp);
    wchar_t* heading = getWC(temp);
    */
    int row = 1;//def 0
    /*String^ query = "INSERT INTO [DB_OnboardFMS].[dbo].[POSITION_REPORT_2] (MACHINE_OID,UNIT,CLASS_NAME,STATUS,NAME,LAST_POSITION_REPORT,X, Y, Z,POSITION_ACCURACY,FIELD_BAD_GPS,SPEED, HEADING) values(" + dholder->machine_oid + ",'" + % String(machine) + "','" + % String(class_name) + "'," + dholder->status + ",'" + % String(activity) + "','" + % String(last_pr) + "'," + % String(x) + "," + % String(y) + "," + % String(z) + "," + % String(accuracy) + "," + dholder->bad_gps + "," + % String(speed) + "," + % String(heading) + ");";
    free(machine);
    free(class_name);
    free(activity);
    free(last_pr);
    free(x);
    free(y);
    free(z);
    free(accuracy);
    free(speed);
    free(heading);
    System::Console::WriteLine(query);
    try {
        SqlConnection sqlConn(connection);
        sqlConn.Open();
        SqlCommand command(query, % sqlConn);
        row = command.ExecuteNonQuery();
        sqlConn.Close();


    }
    catch (Exception^ e) {
        
        System::Console::WriteLine(e->ToString());
    }
    */
    return row;
}
