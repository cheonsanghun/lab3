#include <stdio.h>
#include <stdlib.h>

int main() {
  // 파일을 쓰기 모드로 열기
  FILE *file = fopen("example.txt", "w");

  // 파일이 정상적으로 열렸는지 확인
  if (file == NULL) {
    perror("파일을 열 수 없습니다");
    return 1;
  }

  // 사용자로부터 입력 받기
  printf("파일에 입력할 내용을 입력하세요. 입력이 끝나면 Ctrl+D를 눌러주세요.\n");

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    // 입력 받은 내용을 파일에 쓰기
    fprintf(file, "%s", buffer);
  }

  // 파일 닫기 전에 모든 데이터를 플러시
  fflush(file);

  // 파일 닫기
  fclose(file);

  // 파일을 읽기 모드로 열기
  file = fopen("example.txt", "r");

  // 파일이 정상적으로 열렸는지 확인
  if (file == NULL) {
    perror("파일을 열 수 없습니다");
    return 1;
  }

  // 파일 내용을 화면에 출력
  printf("\n파일 내용:\n");

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    printf("%s", buffer);
  }

  // 파일 닫기
  fclose(file);

  return 0;
}
