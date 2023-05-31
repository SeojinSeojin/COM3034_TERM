# 현재 디렉터리의 모든 자식 디렉터리를 반복
for dir in */ ; do
    # 각 디렉터리에 있는 파일의 수를 센다
    file_count=$(find "$dir" -type f | wc -l)
    # 파일의 수가 59개가 아니면 디렉터리 이름을 출력
    if [ "$file_count" -ne 59 ]; then
        echo "$dir"
    fi
done
