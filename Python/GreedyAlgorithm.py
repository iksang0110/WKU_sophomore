# 입력: 회의의 수와 각 회의의 시작 시간과 끝나는 시간
n = int(input()) # 회의의 수
meetings = []

for _ in range(n):
    start, end = map(int, input().split())
    meetings.append((start, end))

# 끝나는 시간을 기준으로 회의를 정렬
meetings.sort(key=lambda x: (x[1], x[0]))

# 그리디 알고리즘 적용
count = 0
end_time = 0

for meeting in meetings:
    start, end = meeting
    if start >= end_time:
        # 회의를 선택할 수 있는 경우
        end_time = end
        count += 1

print(count)  # 선택된 회의의 최대 개수