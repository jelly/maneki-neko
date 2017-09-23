import socket
from http.server import HTTPServer, SimpleHTTPRequestHandler

import paho.mqtt.publish as publish

SERVER = 'mosquitto.space.revspace.nl'

class MyHandler(SimpleHTTPRequestHandler):
  def do_GET(self):
    if self.path == '/':
      self.send_response(200)
      self.send_header('Content-type', 'text/html')
      self.end_headers()
      self.wfile.write(b'wink')
      publish.single("revspace/winkcat", "wink", hostname=SERVER)
      return
    return SimpleHTTPRequestHandler.do_GET(self)

class HTTPServerV6(HTTPServer):
  address_family = socket.AF_INET6

def main():
  server = HTTPServerV6(('::', 8080), MyHandler)
  server.serve_forever()

if __name__ == '__main__':
  main()

