# //Thanks stackoverflow
# //http://stackoverflow.com/questions/3745666/how-to-convert-from-hex-to-ascii-in-javascript

# function a2hex(str) {
#   var arr = [];
#   for (var i = 0, l = str.length; i < l; i ++) {
#     var hex = Number(str.charCodeAt(i)).toString(16);
#     arr.push(hex);
#   }
#   return arr.join('');
# }

# function hex2a(hexx) {
#     var hex = hexx.toString();//force conversion
#     var str = '';
#     for (var i = 0; i < hex.length; i += 2)
#         str += String.fromCharCode(parseInt(hex.substr(i, 2), 16));
#     return str;
# }

from http.server import CGIHTTPRequestHandler, HTTPServer
from oscrypto import symmetric as osc
import json

class MyHTTPRequestHandler(CGIHTTPRequestHandler):
	def do_GET(self):
		self.send_response(200)
		self.send_header('Content-type', 'text/html; charset=utf-8')
		self.end_headers()

		self.wfile.write(bytes('''
			<h1>Prosty serwer do ajaxa w Pythonie</h1>
			<script src="https://code.jquery.com/jquery-3.3.1.js"
			integrity="sha256-2Kok7MbOyxpgUVvAk/HJ2jigOSYS2auK4Pfzbm7uH60="
			crossorigin="anonymous">
			</script>''', 'utf8'))

	def do_POST(self):
		length = int(self.headers.get('Content-Length'))        
		data_string = self.rfile.read(length)

		#print(data_string.decode('utf8'))

		key = bytes('abcdef1234567890', 'ascii')
		text = data_string.decode('ascii')
		cText = hex2a(text)

		print(cText)

		decodedText = osc.rc4_decrypt(key, a2bytes(cText))
		print(decodedText.decode('utf8'))

		self.send_response(200)
		self.send_header('Content-type', 'text/html')
		self.end_headers()

		self.wfile.write(bytes(a2hex(decodedText.decode('utf8')), 'ascii'))

def a2hex(strr):
	arr = []
	for i in range(0, len(strr)):
		hexx = hex(ord(strr[i]))[-2:]
		arr.append(hexx)
	return ''.join(arr)

def hex2a(hexx):
	if(len(hexx)%2 == 0):
		strr = ''
		for i in range(0, len(hexx)-1, 2):
			strr = strr+str(chr(int(hexx[i:i+2], 16)))
		return strr
	return None

def a2bytes(strr):
	hexx = a2hex(strr)
	bytess = []
	for i in range(0, len(hexx)-1, 2):
		bytess.append(int(hexx[i:i+2], 16))
	return bytes(bytess)

def run(address='localhost', port=8000, server_class=HTTPServer, handler_class=CGIHTTPRequestHandler):
	server_address = (address, port)
	httpd = server_class(server_address, handler_class)
	print("Serving on", address, "with port", port)
	httpd.serve_forever()

if __name__ == "__main__":
	run('localhost', 4443, HTTPServer, MyHTTPRequestHandler)