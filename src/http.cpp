#include "http.h"

#include "Particle.h"
#include "softap_http.h"

struct Page {
    const char* url;
    const char* mime_type;
    const char* data;
};

const char obey_data[] = "<html><body>HeadSlug: <h1>Obey me!</h1><div>You: <a href='/great'>sure</a></div><div>You: <a href='/again'>nope</a></div></body></html>";
const char again_data[] = "<html><body>HeadSlug: <h1>No such option! Obey me!</h1><div>You: <a href='/great'>okay</a></div><div>You: <a href='/again'>nooope</a></div></body></html>";
const char great_data[] = "<html><body>HeadSlug: <h1>That was easy, now put me on your head!</h1></body></html>";

Page slug_pages[] = {
     { "/obey", "text/html", obey_data },
     { "/again", "text/html", again_data },
     { "/great", "text/html", great_data },
     { nullptr }
};

void slugPage(const char* url, ResponseCallback* cb, void* cbArg, Reader* body, Writer* result, void* reserved) {
    Serial.printlnf("handling page %s", url);

    int8_t idx = 0;
    for( ;;idx++ ) {
        Page& p = slug_pages[idx];
        if( ! p.url ) {
            idx = -1;
            break;
        } else if( strcmp(url, p.url) == 0 )
            break;
    }

    if( idx == -1 ) {
        Serial.println("sending redirect");
        Header h("Location: /obey\r\n");
        cb(cbArg, 0, 301, "text/plain", &h);
    } else {
        cb(cbArg, 0, 200, slug_pages[idx].mime_type, nullptr);
        result->write(slug_pages[idx].data);
    }
}

STARTUP(softap_set_application_page_handler(slugPage, nullptr));
