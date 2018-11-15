import http.client

#request to the server the given url. dont use http://
def serverRequest(url, route):
    
    conn = http.client.HTTPConnection(url)

    headers = {'cache-control': "no-cache"}

    conn.request("POST", route, headers=headers)

    res = conn.getresponse()
    data = res.read()

    print(data.decode("utf-8"))

#enable the button on the web server
def enableButton(url):
    serverRequest(url, "/iota/button/enable")

#disable the button on the web server
def disableButton(url):
    serverRequest(url, "/iota/button/disable")

#send the current analog value of the knob to the server
def setKnobVal(url, value):
    serverRequest(url, "/iota/knob?value=" + str(value));