import time
from collections import deque

# 명령어 처리 단계 정의
STAGES = ["Fetch", "Decode", "Execute", "Memory Access", "Write Back"]

# 파이프라인 시뮬레이터 클래스
class PipelineSimulator:
    def __init__(self, instructions):
        self.instructions = instructions  # 처리할 명령어 목록
        self.pipeline = deque([None] * len(STAGES))  # 각 파이프라인 단계 초기화
        self.clock = 0  # 클럭 시간
        self.completed_instructions = []  # 완료된 명령어

    def run(self):
        print("Starting pipeline simulation...\n")

        while self.instructions or any(self.pipeline):
            self.clock += 1  # 클럭 증가
            print(f"Clock Cycle {self.clock}")

            # 파이프라인의 각 단계를 거꾸로 순회하며 명령어를 한 단계씩 이동
            for stage in range(len(STAGES) - 1, 0, -1):
                if self.pipeline[stage - 1] and self.pipeline[stage] is None:
                    self.pipeline[stage] = self.pipeline[stage - 1]
                    self.pipeline[stage - 1] = None

            # 새로운 명령어 Fetch 단계에 추가
            if self.instructions and self.pipeline[0] is None:
                self.pipeline[0] = self.instructions.pop(0)

            # Write Back 단계에서 완료된 명령어 처리
            if self.pipeline[-1]:
                completed_instruction = self.pipeline.pop()
                self.completed_instructions.append(completed_instruction)
                self.pipeline.appendleft(None)
                print(f"Completed: {completed_instruction}")

            # 현재 파이프라인 상태 출력
            self.print_pipeline()
            time.sleep(0.5)  # 진행 속도를 보기 위해 0.5초 지연

        print("\nSimulation completed!")
        print(f"Completed Instructions: {self.completed_instructions}")

    def print_pipeline(self):
        for i, stage in enumerate(STAGES):
            print(f"{stage}: {self.pipeline[i]}")
        print("-" * 30)

# 명령어 목록 생성
instructions = ["Instruction 1", "Instruction 2", "Instruction 3", "Instruction 4", "Instruction 5"]

# 파이프라인 시뮬레이터 실행
simulator = PipelineSimulator(instructions)
simulator.run()
