#include "App_Work.h"
#include "time.h"
#include "cJSON.h"

//位置信息数据
Location_Value location_value;

/**
 * @brief 芯片初始化
 * 
 */
void App_Work_Init(void) {
  //初始化计步芯片
  Int_DS3553_Init();
  //初始化GPS芯片
  Int_AT6558R_Init();
  //初始化联网芯片
  Int_QS100_Init();
}

void __App_Work_GetUid(void){
  //UID是96位二进制，转成24位十六进制
  sprintf((char *)location_value.uid, "%08X%08X%08X", HAL_GetUIDw0(),HAL_GetUIDw1(),HAL_GetUIDw2());
}

void __App_Work_GetStep(void){
  location_value.step = Int_DS3553_GetStepCount();
}

void __App_Work_GetGPS(void){
  //教室无法获取gps准确数据
  // uint8_t gps_string[1000] = {0};
  // Int_AT6558R_GetGPS(gps_string);

  //使用模拟数据
  char *gps_string = "$BDGSV,2,2,05,38,72,326,,0*48\r\n$GNRMC,070822.000,A,4006.81888,N,11621.89413,E,0.81,359.02,020624,,,A,V*02\r\n$GNVTG,359.02,T,,M,0.81,N,1.51,K,A*22";
  char *rmc_string = strstr(gps_string, "$GNRMC") ? "": strstr(gps_string, "$GNRMC")+7;
  char c = 0;//取A位置的值
  sscanf(rmc_string, "%*[^AV]%c", &c);
  if(c == 'A'){
    char time[7] = {0};
    char date[7] = {0};
    double lon = 0;
    double lat = 0;
    //%*表示占位，跳过当前字符
    sscanf(rmc_string, "%6c%*4c,%*c,%lf,%*c,%lf,%*c,%*f,%*f,%6c", time, &lat, &lon, date);
    
    //计算经纬度 4006.81888 = 40+ 06.81888/60
    location_value.lon = (uint8_t)(lon / 100) + ((lon - (uint8_t)(lon / 100) * 100 )/60);
    location_value.lat = (uint8_t)(lat / 100) + ((lat - (uint8_t)(lat / 100) * 100 )/60);

    short year = ( date[4] - 48) * 10 + ( date[5] - 48); // 24
    char mon   = ( date[2] - 48) * 10 + ( date[3] - 48);
    char day   = ( date[0] - 48) * 10 + ( date[1] - 48);
    char hour  = ( time[0] - 48) * 10 + ( time[1] - 48);
    char min   = ( time[2] - 48) * 10 + ( time[3] - 48);
    char sec   = ( time[4] - 48) * 10 + ( time[5] - 48);

    // UTC时间戳（秒）+ 8 * 60 * 60 => Beijing 时间戳（秒）
    struct tm utc_time;
    utc_time.tm_year = year + 100;
    utc_time.tm_mon = mon - 1;
    utc_time.tm_mday = day;
    utc_time.tm_hour = hour;
    utc_time.tm_min = min;
    utc_time.tm_sec = sec;
    //获取时间戳
    time_t timestamp = mktime(&utc_time) + 8 * 60 * 60;
    //转换成北京时间
    struct tm *bj_time = localtime(&timestamp);
    //写入location_value
    sprintf((char *)location_value.date, "%d-%02d-%02d %02d:%02d:%02d", bj_time->tm_year, bj_time->tm_mon,
            bj_time->tm_mday, bj_time->tm_hour, bj_time->tm_min, bj_time->tm_sec);
  } 

}

void __App_Work_SendData(void){
  //封装json数据
  cJSON *json_obj = cJSON_CreateObject();
  cJSON_AddStringToObject(json_obj, "uid", (char *)location_value.uid);
  cJSON_AddStringToObject(json_obj, "date", (char *)location_value.date);
  cJSON_AddNumberToObject(json_obj, "step", location_value.step);
  cJSON_AddNumberToObject(json_obj, "lon", location_value.lon);
  cJSON_AddNumberToObject(json_obj, "lat", location_value.lat);
  char *json = cJSON_PrintUnformatted(json_obj);

  //发送json数据到服务器
  Int_QS100_SendData((uint8_t *)json);
  //释放json数据
  cJSON_free(json);
  cJSON_Delete(json_obj);

}

/**
 * @brief 芯片开始工作
 * 
 */
void App_Work_Execute(void) {
  //获取32芯片id
  __App_Work_GetUid();
  //获取步数
  __App_Work_GetStep();
  //获取位置信息
  __App_Work_GetGPS();
  //发送数据
  __App_Work_SendData();
}


