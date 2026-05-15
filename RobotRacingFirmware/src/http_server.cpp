#include "http_server.hpp"
#include <esp_http_server.h>

void HTTPServer::init(Camera *camera, Drive *drive, String name) {
    this->camera = camera;
    this->drive = drive;
    this->name = name;
}

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";

static esp_err_t streamHandler(httpd_req_t *req) {
    Camera* camera = static_cast<Camera*>(req->user_ctx);
    camera_fb_t *fb = NULL;
    char buffer[128];
    esp_err_t res = ESP_OK;

    res = httpd_resp_set_type(req, STREAM_CONTENT_TYPE);
    if (res != ESP_OK) {
        return res;
    }

    while (true) {
        fb = camera->capture();
        if (!fb) {
            Serial.println("Camera capture failed");
            break;
        }
        if (res == ESP_OK) {
            res = httpd_resp_send_chunk(req, STREAM_BOUNDARY, strlen(STREAM_BOUNDARY));
        }
        if (res == ESP_OK) {
            size_t hlen = snprintf((char *)buffer, 128, STREAM_PART, fb->len, fb->timestamp.tv_sec, fb->timestamp.tv_usec);
            res = httpd_resp_send_chunk(req, (const char *)buffer, hlen);
        }
        if (res == ESP_OK) {
            res = httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
        }
        esp_camera_fb_return(fb);

        if (res != ESP_OK) {
            Serial.printf("Error sending stream chunk: %d\n", res);
            break;
        }
    }

    return res;
}

static esp_err_t driveHandler(httpd_req_t *req) {
    Drive* drive = static_cast<Drive*>(req->user_ctx);
    
    char query[128];
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) != ESP_OK)
    {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    char value[16];
    if (httpd_query_key_value(query, "left", value, sizeof(value)) != ESP_OK) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    int left = atoi(value);

    if (httpd_query_key_value(query, "right", value, sizeof(value)) != ESP_OK){
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    int right = atoi(value);

    drive->setSpeed(left, right);
    return httpd_resp_send(req, "OK", 2);
}

void HTTPServer::startServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 2;

    httpd_handle_t controlServer = nullptr;
    httpd_handle_t streamServer = nullptr;

    httpd_uri_t streamURI = {
        .uri       = "/stream",
        .method    = HTTP_GET,
        .handler   = streamHandler,
        .user_ctx  = camera
    };

    httpd_uri_t driveURI = {
        .uri       = "/drive",
        .method    = HTTP_GET,
        .handler   = driveHandler,
        .user_ctx  = drive
    };

    httpd_uri_t pingURI = {
        .uri       = "/name",
        .method    = HTTP_GET,
        .handler   = [](httpd_req_t *req) -> esp_err_t {
            const char* name = static_cast<const char*>(req->user_ctx);
            httpd_resp_send(req, name, strlen(name));
            return ESP_OK;
        },
        .user_ctx  = (void*)name.c_str()
    };


    if (httpd_start(&controlServer, &config) == ESP_OK) {
        httpd_register_uri_handler(controlServer, &driveURI);
        httpd_register_uri_handler(controlServer, &pingURI);
    }

    config.server_port += 1;
    config.ctrl_port += 1;
    if (httpd_start(&streamServer, &config) == ESP_OK) {
        httpd_register_uri_handler(streamServer, &streamURI);
    }
}

