#!/usr/bin/python2
# -*- coding: UTF-8 -*-

# from urllib.request import urlopen
import urllib.request
import base64
import json
import urllib.parse

## 获取订阅地址
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:23.0) Gecko/20100101 Firefox/23.0'}
subscribe_url = 'https://www.xiaohouzila.site/link/NZFvzw0ZS49a1KI8?mu=2'
req = urllib.request.Request(url=subscribe_url, headers=headers)
return_content = urllib.request.urlopen(req).read()

#print(return_content)
## 解析订阅地址内容
lens = len(return_content)
lenx = lens - (lens % 4 if lens % 4 else 0)
configs = []
try:
    result = base64.decodestring(return_content[:lenx])
    #print(result)
    share_links=result.splitlines()
    ## 解析vmess协议
    schemes_allow = ['vmess', 'ss', 'socks']
    index = 0
    for share_link in share_links:
        print(index)
        index = index+1
        share_link=bytes.decode(share_link)
        print(share_link)
        url=share_link.split("://")
        ## 解析协议
        scheme=url[0]
        #print(scheme)
        if scheme not in schemes_allow:
            print(scheme+"不支持")
            continue
        ## 解析内容
        net=url[1]
        net=str.encode(net)
        lens = len(net)
        print(lens)
        lenx = lens - (lens % 4 if lens % 4 else 0)
        resultJson = base64.decodestring(net[:lenx])
        print(resultJson)
        configs.append(json.loads(bytes.decode(resultJson)))
        #json.loads(resultJson)
except Exception as e :
   print(e)
   pass

print(configs)
try:
    with open("sites.json", "w") as dump_f:
        json.dump(configs, dump_f)
except Exception as e :
   print(e)
   pass
