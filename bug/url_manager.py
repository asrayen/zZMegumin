from msilib import MSIMODIFY_VALIDATE
import requests
import json


def get_pages(n):
    cookies = {
        'i-wanna-go-back': '-1',
        'LIVE_BUVID': 'AUTO5816549258274507',
        'buvid_fp_plain': 'undefined',
        'buvid4':
        '6E82B606-F332-FE64-3C7B-5D66AC1EBA8119268-022061113-fs30e%2FwXVzGLn6KOCcb6NgwHRJz9Z3LXG%2Ba4q1YQleZe9KXFCZrxPw%3D%3D',
        'rpdid': '|(u||uu~JuYk0J\'uYY)l~kuR|',
        'CURRENT_PID': '8f19f9c0-ce16-11ed-8b47-df2302daf0d7',
        'is-2022-channel': '1',
        'CURRENT_QUALITY': '112',
        'b_ut': '5',
        'buvid3': '146615E6-ADB9-F669-724E-355ECCD9047A95526infoc',
        'b_nut': '1686467695',
        '_uuid': '854B84E6-E16D-1648-67F10-7A78B10CC4F9996359infoc',
        'hit-dyn-v2': '1',
        'FEED_LIVE_VERSION': 'V8',
        'hit-new-style-dyn': '1',
        'header_theme_version': 'CLOSE',
        'enable_web_push': 'DISABLE',
        'fingerprint': '97e72dd5737d860e3f5e17eed11ccf26',
        'CURRENT_BLACKGAP': '0',
        'CURRENT_FNVAL': '4048',
        'home_feed_column': '4',
        'bili_ticket':
        'eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3MDAyMTI1OTMsImlhdCI6MTY5OTk1MzMzMywicGx0IjotMX0.mcOhSOENdMJniO5fKyZSwuIzROqtmE_bWX7SBg3L6MU',
        'bili_ticket_expires': '1700212533',
        'browser_resolution': '1272-598',
        'share_source_origin': 'QQ',
        'bsource': 'share_source_qqchat',
        'PVID': '2',
        'SESSDATA':
        '0a71fa2e%2C1715694052%2Ca5d3e%2Ab1CjCQ3H04ZIuDo5qqr0g0rlTHQcVKty8dcJ39_iKxddtQRv_hL1sPntILSU0ZNt4_vi0SVnBVTXdRMS1QTzVfSGhjSkYxVjVTZVpncUdZS3cxMFpyWTFzUWJ6UDBWY2lJRDZKMm1jaHZ3Z2NpdlFibzY3QUxKY2FMUDI4aUdvaDZiWDBGYml5Q1pRIIEC',
        'bili_jct': 'c6a6e3003f376e2ae37b1a0e06fee9d8',
        'DedeUserID': '340863448',
        'DedeUserID__ckMd5': '3fb8850e7e0778dd',
        'sid': '7exuhrp3',
        'buvid_fp': '97e72dd5737d860e3f5e17eed11ccf26',
        'bp_video_offset_340863448': '864594019263447045',
        'b_lsid': '104795F9F_18BD8DCAAB8',
    }

    headers = {
        'authority':
        'api.bilibili.com',
        'accept':
        '*/*',
        'accept-language':
        'zh-CN,zh;q=0.9',
        # Requests sorts cookies= alphabetically
        # 'cookie': 'i-wanna-go-back=-1; LIVE_BUVID=AUTO5816549258274507; buvid_fp_plain=undefined; buvid4=6E82B606-F332-FE64-3C7B-5D66AC1EBA8119268-022061113-fs30e%2FwXVzGLn6KOCcb6NgwHRJz9Z3LXG%2Ba4q1YQleZe9KXFCZrxPw%3D%3D; rpdid=|(u||uu~JuYk0J\'uYY)l~kuR|; CURRENT_PID=8f19f9c0-ce16-11ed-8b47-df2302daf0d7; is-2022-channel=1; CURRENT_QUALITY=112; b_ut=5; buvid3=146615E6-ADB9-F669-724E-355ECCD9047A95526infoc; b_nut=1686467695; _uuid=854B84E6-E16D-1648-67F10-7A78B10CC4F9996359infoc; hit-dyn-v2=1; FEED_LIVE_VERSION=V8; hit-new-style-dyn=1; header_theme_version=CLOSE; enable_web_push=DISABLE; fingerprint=97e72dd5737d860e3f5e17eed11ccf26; CURRENT_BLACKGAP=0; CURRENT_FNVAL=4048; home_feed_column=4; bili_ticket=eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3MDAyMTI1OTMsImlhdCI6MTY5OTk1MzMzMywicGx0IjotMX0.mcOhSOENdMJniO5fKyZSwuIzROqtmE_bWX7SBg3L6MU; bili_ticket_expires=1700212533; browser_resolution=1272-598; share_source_origin=QQ; bsource=share_source_qqchat; PVID=2; SESSDATA=0a71fa2e%2C1715694052%2Ca5d3e%2Ab1CjCQ3H04ZIuDo5qqr0g0rlTHQcVKty8dcJ39_iKxddtQRv_hL1sPntILSU0ZNt4_vi0SVnBVTXdRMS1QTzVfSGhjSkYxVjVTZVpncUdZS3cxMFpyWTFzUWJ6UDBWY2lJRDZKMm1jaHZ3Z2NpdlFibzY3QUxKY2FMUDI4aUdvaDZiWDBGYml5Q1pRIIEC; bili_jct=c6a6e3003f376e2ae37b1a0e06fee9d8; DedeUserID=340863448; DedeUserID__ckMd5=3fb8850e7e0778dd; sid=7exuhrp3; buvid_fp=97e72dd5737d860e3f5e17eed11ccf26; bp_video_offset_340863448=864594019263447045; b_lsid=104795F9F_18BD8DCAAB8',
        'origin':
        'https://space.bilibili.com',
        'referer':
        'https://space.bilibili.com/334934029/fans/fans',
        'sec-ch-ua':
        '"Microsoft Edge";v="119", "Chromium";v="119", "Not?A_Brand";v="24"',
        'sec-ch-ua-mobile':
        '?0',
        'sec-ch-ua-platform':
        '"Windows"',
        'sec-fetch-dest':
        'empty',
        'sec-fetch-mode':
        'cors',
        'sec-fetch-site':
        'same-site',
        'user-agent':
        'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 Edg/119.0.0.0',
    }

    params = (('vmid', '334934029'), ('pn', str(n)), ('ps', '20'), ('order',
                                                                    'desc'))

    response = requests.get('https://api.bilibili.com/x/relation/fans',
                            params=params,
                            cookies=cookies,
                            headers=headers)
    return response


time = 0
for i in range(1, 6):
    r = get_pages(i)
    json_obj = json.loads(r.text)
    for entry in json_obj['data']['list']:
        myid = 334934029
        uname = entry['uname']
        mid = entry['mid']
        label = '粉丝'
        time = time + 1
        print(mid, myid, label)
        # print(mid, uname, 0)
