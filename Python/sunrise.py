import folium
from geopy.distance import geodesic

# 출발 위치 구글맵 좌표값찾아서 넣기. 이건 자동화 할 수 있을 것 같음
chonnam_univ = {'name': '전남대학교', 'lat': 35.174442, 'lon': 126.908621}


locations = [# 지역 적기 위치값은 구글맵을 통해 확인햐기
    {'name': '향일암', 'lat': 34.6442, 'lon': 127.7409},
    {'name': '오동도', 'lat': 34.7380, 'lon': 127.6622},
    {'name': '하멜등대', 'lat': 34.7475, 'lon': 127.7376},
    {'name': '돌산공원', 'lat': 34.7366, 'lon': 127.6618},
    {'name': '만성리 검은모래해변', 'lat': 34.7895, 'lon': 127.6543},
    {'name': '남열해돋이해수욕장', 'lat': 34.5250, 'lon': 127.4560},
    {'name': '고흥우주발사전망대', 'lat': 34.5255, 'lon': 127.4565},
    {'name': '나로우주센터우주과학관', 'lat': 34.4311, 'lon': 127.5350},
    {'name': '화포해변', 'lat': 34.8605, 'lon': 127.4911},
    {'name': '순천만 국가정원', 'lat': 34.9685, 'lon': 127.5093},
    {'name': '구봉산 전망대', 'lat': 34.9402, 'lon': 127.5810},
    {'name': '배알도 수변공원', 'lat': 34.9333, 'lon': 127.7022},
    {'name': '망덕포구', 'lat': 34.9333, 'lon': 127.7022}
]


m = folium.Map(location=[chonnam_univ['lat'], chonnam_univ['lon']], zoom_start=10)


folium.Marker(
    # 출발위치 마커 추가
    [chonnam_univ['lat'], chonnam_univ['lon']],
    popup=chonnam_univ['name'],
    icon=folium.Icon(color='red')
).add_to(m)

for loc in locations:
   
    folium.Marker(
        [loc['lat'], loc['lon']],
        icon=folium.Icon(color='blue')
    ).add_to(m)
    
    
    folium.map.Marker(
        [loc['lat'], loc['lon']],
        icon=folium.DivIcon(
            html=f"""<div style="font-size: 12pt; color: black; white-space: nowrap;">{loc['name']}</div>"""
        )
    ).add_to(m)
    
    # 출발지에서 장소까지의 거리 계산 (단위: km)
    distance = geodesic(
        (chonnam_univ['lat'], chonnam_univ['lon']),
        (loc['lat'], loc['lon'])
    ).km
    
    # 평균 속도 60km/h로 가정하여 소요 시간 계산 (단위: 시간)
    travel_time = distance / 60
    
  
    folium.PolyLine(
        locations=[
            [chonnam_univ['lat'], chonnam_univ['lon']],
            [loc['lat'], loc['lon']]
        ],
        color='green',
        weight=2.5,
        opacity=1
    ).add_to(m)
    

    mid_lat = (chonnam_univ['lat'] + loc['lat']) / 2
    mid_lon = (chonnam_univ['lon'] + loc['lon']) / 2
    folium.map.Marker(
        [mid_lat, mid_lon],
        icon=folium.DivIcon(
            html=f"""<div style="font-size: 10pt; color: black; white-space: nowrap;">{distance:.1f} km<br>{travel_time:.1f} 시간</div>"""
        )
    ).add_to(m)


folium.Circle(
    radius=100000,  # 반경 200km (단위: 미터)
    location=[chonnam_univ['lat'], chonnam_univ['lon']],
    color='crimson',
    fill=True,
    fill_opacity=0.1
).add_to(m)

# 지도 저장
m.save('map.html')