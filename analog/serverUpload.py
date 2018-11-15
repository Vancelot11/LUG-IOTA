import http.client

#request to the server the given url
def serverRequest(url):
    
    conn = http.client.HTTPConnection("flip3.engr.oregonstate.edu:2031")

    headers = {'cache-control': "no-cache"}

    conn.request("POST", url, headers=headers)

    res = conn.getresponse()
    data = res.read()

    print(data.decode("utf-8"))

#enable the button on the web server
def enableButton():
    button("/iota/button/enable")

#disable the button on the web server
def disableButton():
    button("/iota/button/disable")
