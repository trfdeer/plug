#include "routes.h"
#include "utils.h"

#include "ESP8266WebServer.h"

#include <charconv>
#include <utility>

#define HOME_PAGE                                                              \
  "<!DOCTYPE html><html lang=\"en\"> <head> <meta charset=\"UTF-8\"/> <meta "  \
  "http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"/> <meta "                \
  "name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/> "     \
  "<title>Plug Control</title> <style>/* Classless.css v1.0 (Tiny Inline "     \
  "Version)*/ *{box-sizing: border-box; border-spacing: 0;}header, footer, "   \
  "figure, table, video, details, blockquote, ul, ol, dl, fieldset, pre, pre " \
  "> code, caption, nav{display: block; margin: 0.5rem 0rem 1rem; width: "     \
  "100%; overflow: auto hidden; text-align: left;}video, summary, input, "     \
  "select{outline: none;}html{font: 12pt/1.6 \"Open Sans\", Helvetica, "       \
  "sans-serif;}body{position: relative; margin: auto; max-width: 50rem; "      \
  "color: #433; padding: 3.1rem 0.6rem 0; overflow-x: hidden;}body > "         \
  "footer{margin: 10rem 0rem 0rem;}/* clickable stuff */ a, summary, button, " \
  "select{color: #07c; cursor: pointer;}a{text-decoration: underline solid "   \
  "#d1d1d1; text-underline-position: under;}a:hover{color: #088; "             \
  "border-color: #088;}/* common */ td, th, pre > code{padding: 0.5rem "       \
  "0.8rem;}.card, details{padding: 0 0.6rem;}code, kbd, samp{padding: "        \
  "0.2rem; font: 0.9em/1.4 monospace;}code, kbd, samp, nav, "                  \
  ".card{background: #f4f5f6; border-radius: 0.3em;}kbd, .card, "              \
  "details[open]{border: 1px solid #d1d1d1;}td, th, figcaption, caption, "     \
  ".card{font-size: 95%;}td, th, hr{border: 0; border-bottom: 0.1rem solid "   \
  "#d1d1d1;}/* lists */ ul, ol{padding-top: 0.5rem;}li, dd{margin-bottom: "    \
  "0.5rem;}dt{font-weight: bold;}/* headings */ h1, h2, h3, h4, h5{margin: "   \
  "1.5em 0 0em; line-height: 1.2em;}h1 + h2, h2 + h3, h3 + h4, h4 + "          \
  "h5{margin-top: 0.5em;}h1{font-size: 2.2em; font-weight: "                   \
  "300;}h2{font-size: 2em; font-weight: 300; font-variant-caps: "              \
  "small-caps;}h3{font-size: 1.5em; font-weight: 400;}h4{font-size: 1.1em; "   \
  "font-weight: 700;}h5{font-size: 1.2em; font-weight: 400; color: "           \
  "#888;}h6{font-size: 1em; font-weight: 600; display: inline;}h6 + "          \
  "p{display: inline;}/* tables */ td, th{text-align: right; white-space: "    \
  "nowrap;}td:first-child, th:first-child{text-align: "                        \
  "left;}tr:hover{background-color: #f4f5f6;}/* figures */ img, "              \
  "svg{max-width: 100%; vertical-align: text-top;}p > "                        \
  "img:not(:only-child){float: right; margin: 0 0 0.5em 0.5em;}figure > "      \
  "img{display: block; margin: 0.5em auto;}figcaption, caption{color: #888; "  \
  "margin-bottom: 1rem;}figure > *:not(:last-child){margin: 0 0 "              \
  "0.4rem;}/*code*/ pre > code{margin: 0; border-left: 0.4rem solid #088;}/* " \
  "misc */ blockquote{border-left: 0.3rem solid #d1d1d1; padding: 1rem "       \
  "1.5rem; font-style: italic;}input{font-size: 1em;}p > "                     \
  "cite:before{content: \" (\";}p > cite:after{content: \") \";}/* grid */ "   \
  ".row{display: flex; margin: 0.5rem -0.5rem; align-items: stretch;}.row "    \
  "[class*=\"col\"]{padding: 0 0.5rem; flex: 1 1 100%;}@media (max-width: "    \
  "40em){.row{flex-direction: column !important;}}.plug{display: "             \
  "inline-flex; justify-content: center; align-items: center; font-size: "     \
  "3em; user-select: none; box-shadow: 0px 0px 5px 0px rgba(0, 0, 0, 0.75); "  \
  "-webkit-box-shadow: 0px 0px 5px 0px rgba(0, 0, 0, 0.75); -moz-box-shadow: " \
  "0px 0px 5px 0px rgba(0, 0, 0, 0.75); transition: all 300ms; cursor: "       \
  "pointer; width: 200px; height: 200px; margin: 5px; border-radius: "         \
  "100%;}.plug:hover{filter: brightness(110%);}.plug-on{background-color: "    \
  "#45e695;}.plug-off{background-color: "                                      \
  "#d85963;}.plug-inactive{background-color: #c7c1c2;}</style> </head> "       \
  "<body> <div style=\"margin-bottom: 50px\"> <h1 style=\"text-align: "        \
  "center\">Plug Controls</h1> </div><div style=\"display: flex; "             \
  "justify-content: space-evenly; flex-wrap: wrap\"> <div class=\"plug "       \
  "plug-inactive\" id=\"plug1\" data-id=\"1\">1</div><div class=\"plug "       \
  "plug-inactive\" id=\"plug2\" data-id=\"2\">2</div><div class=\"plug "       \
  "plug-inactive\" id=\"plug3\" data-id=\"3\">3</div></div><script>const "     \
  "refreshStatus=(id, plug)=>{fetch(`/status?id=${id}`) .then((resp)=> "       \
  "resp.text()) .then((data)=>{plug.classList.remove(\"plug-inactive\"); "     \
  "plug.classList.remove(\"plug-on\"); plug.classList.remove(\"plug-off\"); "  \
  "if (data==\"ON\") plug.classList.add(\"plug-on\"); else if "                \
  "(data==\"OFF\") plug.classList.add(\"plug-off\"); "                         \
  "else{plug.classList.add(\"plug-inactive\"); alert(data);}}) "               \
  ".catch((err)=> alert(`Failed to get status for Plug ${id}: ${err}`));}; "   \
  "document.querySelectorAll(\".plug\").forEach((plug)=>{const "               \
  "id=plug.dataset.id; const elId=`plug${id}`; refreshStatus(id, plug); "      \
  "plug.addEventListener(\"click\", ()=>{fetch(`/toggle?id=${id}`) "           \
  ".then((resp)=> resp.text()) .then((data)=>{if (data "                       \
  "!=\"OK\"){alert(data);}else{refreshStatus(id, plug);}}) .catch((err)=> "    \
  "alert(`Failed to get status for Plug ${id}: ${err}`) );});}); </script> "   \
  "</body></html>"

using std::pair, std::string, std::errc, std::from_chars,
    std::from_chars_result;

extern ESP8266WebServer webServer;

pair<bool, uint8_t> get_pin() {
  if (!webServer.hasArg("id")) {
    webServer.send(400, "text/plain", "missing required parameter: 'id'");
    return {false, -1};
  }

  auto id_str = webServer.arg("id");
  int id{};
  from_chars_result res = from_chars(id_str.begin(), id_str.end(), id);
  if (res.ec == errc::invalid_argument || res.ec == errc::result_out_of_range) {
    webServer.send(400, "text/plain", "invalid value for parameter: 'id'");
    return {false, -1};
  }

  auto pin = id_to_pin(id);
  if (pin == -1) {
    webServer.send(400, "text/plain", "no plug with given id");
    return {false, -1};
  }

  return {true, pin};
}

void routes::home() { webServer.send(200, "text/html", HOME_PAGE); }

void routes::not_found() {
  webServer.sendHeader("Location", "/", true);
  webServer.send(302, "text/plain", "");
}

void routes::plug_on() {
  auto res = get_pin();
  if (!res.first) {
    return;
  }

  auto pin = res.second;
  digitalWrite(pin, LOW);
  webServer.send(200, "text/plain", "OK");
}

void routes::plug_off() {
  auto res = get_pin();
  if (!res.first) {
    return;
  }

  auto pin = res.second;
  digitalWrite(pin, HIGH);
  webServer.send(200, "text/plain", "OK");
}

void routes::plug_toggle() {
  auto res = get_pin();
  if (!res.first) {
    return;
  }

  auto pin = res.second;
  auto status = digitalRead(pin);

  if (status == LOW)
    digitalWrite(pin, HIGH);
  else
    digitalWrite(pin, LOW);
  webServer.send(200, "text/plain", "OK");
}

void routes::plug_status() {
  auto res = get_pin();
  if (!res.first) {
    return;
  }

  auto pin = res.second;
  auto status = digitalRead(pin);

  if (status == LOW)
    webServer.send(200, "text/plain", "ON");
  else
    webServer.send(200, "text/plain", "OFF");
}