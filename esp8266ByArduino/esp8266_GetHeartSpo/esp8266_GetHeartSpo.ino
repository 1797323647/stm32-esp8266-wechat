#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//3.14 16：26更改

// Wi-Fi 凭据
const char* ssid = "TP-LINK_029D";          // 您的Wi-Fi名称
const char* password = "qwertyuiop";          // 您的Wi-Fi密码

// OneNet 平台参数
const char* mqtt_server = "mqtts.heclouds.com"; // OneNet MQTT地址
const int mqtt_port = 1883;                      // 未加密端口
const char* product_id = "3VQ6EZjF8b";          // 产品ID
const char* device_name = "ESP8266";          // 设备名称
const char* api_key = "version=2018-10-31&res=products%2F3VQ6EZjF8b%2Fdevices%2FESP8266&et=1837255523&method=md5&sign=rF%2BYg2QXYB3hz56G1WWE2Q%3D%3D"; // 替换为您的API Key  **请替换为您的API Key**

// 定义发布和订阅主题
String pub_topic = "$sys/" + String(product_id) + "/" + String(device_name) + "/dp/post/json";
String sub_topic = "$sys/" + String(product_id) + "/" + String(device_name) + "/cmd/request/#";

WiFiClient espClient;
PubSubClient client(espClient);

// 回调函数，处理接收到的消息
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("收到消息 [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// 连接Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("连接到 ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi 已连接");
  Serial.print("IP 地址: ");
  Serial.println(WiFi.localIP());
}

// 连接OneNet MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("串口正常》》》》》");
    Serial.print("尝试连接 MQTT...");
    String clientId = "ESP8266"; // 固定 Client ID
    String username = product_id;
    String password_mqtt = api_key;

    if (client.connect(clientId.c_str(), username.c_str(), password_mqtt.c_str())) {
      Serial.println("已连接到 OneNet MQTT");
      // 订阅主题
      client.subscribe(sub_topic.c_str());
      Serial.println("已订阅: " + sub_topic);
    } else {
      Serial.print("连接失败, rc=");
      Serial.print(client.state());
      Serial.println(" 5秒后重试");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200); // 初始化ESP8266串口，与STM32 UART2波特率一致
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); // 设置回调函数
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 接收来自 STM32 的数据并上传
  if (Serial.available() > 0) {
    String dataFromSTM32 = Serial.readStringUntil('\n'); // 读取STM32发送的数据，以换行符结束
    dataFromSTM32.trim(); // 去除首尾空格和换行符

    Serial.print("接收到 STM32 数据: ");
    Serial.println(dataFromSTM32);

    // 判断数据类型：心率血氧数据 或 MPU6050 数据
    if (dataFromSTM32.startsWith("HR:")) {
      // 解析心率血氧数据，格式 "HR:XX,SpO2:YY"
      int hr_index = dataFromSTM32.indexOf("HR:");
      int spo2_index = dataFromSTM32.indexOf(",SpO2:");

      if (hr_index != -1 && spo2_index != -1) {
        String hr_str = dataFromSTM32.substring(hr_index + 3, spo2_index); // 提取 HR 值
        String spo2_str = dataFromSTM32.substring(spo2_index + 6);          // 提取 SpO2 值

        hr_str.trim();
        spo2_str.trim();

        int heartRateValue = hr_str.toInt();
        int spo2Value = spo2_str.toInt();

        Serial.print("解析得到 - 心率: ");
        Serial.print(heartRateValue);
        Serial.print(", 血氧: ");
        Serial.println(spo2Value);

        // 创建 JSON 文档对象，指定容量为512字节
        StaticJsonDocument<512> doc;
        doc["id"] = 123; // 添加 id 字段
        JsonObject dp = doc.createNestedObject("dp"); // 创建 dp 对象

        // 添加 心率 数据点
        JsonArray dis_hr_array = dp.createNestedArray("dis_hr"); // 创建 dis_hr 数组
        JsonObject dis_hr_object = dis_hr_array.createNestedObject(); // 创建 dis_hr 数组中的对象
        dis_hr_object["v"] = heartRateValue; // 设置心率值，字段名为 "v"

        // 添加 血氧 数据点
        JsonArray dis_spo2_array = dp.createNestedArray("dis_spo2"); // 创建 dis_spo2 数组
        JsonObject dis_spo2_object = dis_spo2_array.createNestedObject(); // 创建 dis_spo2 数组中的对象
        dis_spo2_object["v"] = spo2Value; // 设置血氧值

        char payload[512]; // 定义 JSON 字符串缓冲区，增大缓冲区大小
        serializeJson(doc, payload, sizeof(payload)); // 将 JSON 文档序列化为字符串

        if (client.publish(pub_topic.c_str(), payload)) { // 发布 JSON 消息
          Serial.print("已发布 HR/SpO2 消息到主题: ");
          Serial.println(pub_topic);
          Serial.print("消息内容: ");
          Serial.println(payload);
          Serial.print("已发布心率: "); // 打印发布的心率/血氧值
          Serial.print(heartRateValue);
          Serial.print(", 血氧: ");
          Serial.println(spo2Value);
        } else {
          Serial.println("HR/SpO2 消息发布失败");
        }
      } else {
        Serial.println("HR/SpO2 数据格式不正确，无法解析 HR 和 SpO2 值");
        Serial.println("接收到的原始数据: " + dataFromSTM32); // 打印接收到的原始数据，方便调试
      }
    } else if (dataFromSTM32.startsWith("AX:")) {
      // 解析 MPU6050 数据，格式 "AX:XXX,AY:YYY,AZ:ZZZ,GX:AAA,GY:BBB,GZ:CCC"
      int ax_index = dataFromSTM32.indexOf("AX:");
      int ay_index = dataFromSTM32.indexOf(",AY:");
      int az_index = dataFromSTM32.indexOf(",AZ:");
      int gx_index = dataFromSTM32.indexOf(",GX:");
      int gy_index = dataFromSTM32.indexOf(",GY:");
      int gz_index = dataFromSTM32.indexOf(",GZ:");

      if (ax_index != -1 && ay_index != -1 && az_index != -1 && gx_index != -1 && gy_index != -1 && gz_index != -1) {
        String ax_str = dataFromSTM32.substring(ax_index + 3, ay_index); // 提取 AX 值
        String ay_str = dataFromSTM32.substring(ay_index + 4, az_index); // 提取 AY 值
        String az_str = dataFromSTM32.substring(az_index + 4, gx_index); // 提取 AZ 值
        String gx_str = dataFromSTM32.substring(gx_index + 4, gy_index); // 提取 GX 值
        String gy_str = dataFromSTM32.substring(gy_index + 4, gz_index); // 提取 GY 值
        String gz_str = dataFromSTM32.substring(gz_index + 4);           // 提取 GZ 值

        ax_str.trim();
        ay_str.trim();
        az_str.trim();
        gx_str.trim();
        gy_str.trim();
        gz_str.trim();

        int axValue = ax_str.toInt();
        int ayValue = ay_str.toInt();
        int azValue = az_str.toInt();
        int gxValue = gx_str.toInt();
        int gyValue = gy_str.toInt();
        int gzValue = gz_str.toInt();

        Serial.print("解析得到 - AX: "); Serial.print(axValue);
        Serial.print(", AY: "); Serial.print(ayValue);
        Serial.print(", AZ: "); Serial.println(azValue);
        Serial.print("解析得到 - GX: "); Serial.print(gxValue);
        Serial.print(", GY: "); Serial.print(gyValue);
        Serial.print(", GZ: "); Serial.println(gzValue);


        // 创建 JSON 文档对象，指定容量为512字节
        StaticJsonDocument<512> doc;
        doc["id"] = 123; // 添加 id 字段
        JsonObject dp = doc.createNestedObject("dp"); // 创建 dp 对象

        // 添加 MPU6050 数据点
        JsonArray ax_array = dp.createNestedArray("AX");
        JsonObject ax_object = ax_array.createNestedObject();
        ax_object["v"] = axValue;

        JsonArray ay_array = dp.createNestedArray("AY");
        JsonObject ay_object = ay_array.createNestedObject();
        ay_object["v"] = ayValue;

        JsonArray az_array = dp.createNestedArray("AZ");
        JsonObject az_object = az_array.createNestedObject();
        az_object["v"] = azValue;

        JsonArray gx_array = dp.createNestedArray("GX");
        JsonObject gx_object = gx_array.createNestedObject();
        gx_object["v"] = gxValue;

        JsonArray gy_array = dp.createNestedArray("GY");
        JsonObject gy_object = gy_array.createNestedObject();
        gy_object["v"] = gyValue;

        JsonArray gz_array = dp.createNestedArray("GZ");
        JsonObject gz_object = gz_array.createNestedObject();
        gz_object["v"] = gzValue;


        char payload[512]; // 定义 JSON 字符串缓冲区，增大缓冲区大小
        serializeJson(doc, payload, sizeof(payload)); // 将 JSON 文档序列化为字符串

        if (client.publish(pub_topic.c_str(), payload)) { // 发布 JSON 消息
          Serial.print("已发布 MPU6050 消息到主题: ");
          Serial.println(pub_topic);
          Serial.print("消息内容: ");
          Serial.println(payload);
          Serial.print("已发布 MPU6050 数据 - AX: "); Serial.print(axValue);
          Serial.print(", AY: "); Serial.print(ayValue);
          Serial.print(", AZ: "); Serial.println(azValue);
          Serial.print(" GX: "); Serial.print(gxValue);
          Serial.print(", GY: "); Serial.print(gyValue);
          Serial.print(", GZ: "); Serial.println(gzValue);

        } else {
          Serial.println("MPU6050 消息发布失败");
        }
      } else {
        Serial.println("MPU6050 数据格式不正确，无法解析 MPU6050 值");
        Serial.println("接收到的原始数据: " + dataFromSTM32); // 打印接收到的原始数据，方便调试
      }

    } else {
      Serial.println("未知数据类型，无法解析");
      Serial.println("接收到的原始数据: " + dataFromSTM32); // 打印接收到的原始数据，方便调试
    }
  }
  delay(100); // 延时
}