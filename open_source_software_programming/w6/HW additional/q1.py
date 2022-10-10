import random


class PolicyIteration:
    def __init__(self, env):
        # 환경에 대한 객체 선언
        self.possible_actions = [0, 1, 2, 3, 4, 5]  # 좌, 우, 상, 하, 탑승, 하차
        self.env = env
        self.height = 5
        self.width = 5
        self.passenger_loc_num = 5
        self.dest_num = 4
        self.all_states = []
        # 500 가지의 경우의 수에 대한 초기화
        # 차량위치_가로(5) X 차량위치_세로(5) X 승객 위치 (5) X 목적지 위치 (4)
        self.value_table = []
        for a in range(self.width * self.height * self.passenger_loc_num * self.dest_num):
            self.value_table.append(0.0)
        self.policy_table = []
        # 상 하 좌 우 탑승 하차 동일한 확률로 정책 초기화
        for a in range(self.width * self.height * self.passenger_loc_num * self.dest_num):
            self.policy_table.append([1/6, 1/6, 1/6, 1/6, 1/6, 1/6])
        # 감가율 default
        self.discount_factor = 0.9
        # 모든 상태를 리스트화
        for a in range(self.width):
            for b in range(self.height):
                for c in range(self.passenger_loc_num):
                    for d in range(self.dest_num):
                        self.all_states.append((a, b, c, d))

    # 새로운 env 로 진행하기 위해 policy 와 value 초기화
    def reset(self, env):
        self.env = env
        self.value_table = []
        for a in range(self.width * self.height * self.passenger_loc_num * self.dest_num):
            self.value_table.append(0.0)
        self.policy_table = []
        for a in range(self.width * self.height * self.passenger_loc_num * self.dest_num):
            self.policy_table.append([1/6, 1/6, 1/6, 1/6, 1/6, 1/6])

    # policy stabilize 를 위한 코드
    def policy_stable(self, discount_factor, theta):
        iter_max = 9999  # 2단계 evaluation 의 최대 반복횟수
        self.discount_factor = discount_factor
        result = [False, 0, 0]  # [성공여부, improvement 횟수, evaluation 횟수]
        for i in range(100):  # 최대 policy_improvement 도전횟수
            for j in range(iter_max):
                ret = self.policy_evaluate()  # ret: max( value_delta )
                if (ret < theta):  # theta 보다 작을 경우 몇회 진행했는지 기록에 합산, evaluation 종료
                    result[2] += j+1
                    break
            if self.policy_improvement():  # improvement의 반환: policy 변동하지 않았을 때 True
                result[1] = i + 1  # policy improvement 횟수
                result[0] = True  # policy 변동 없을 경우 stabilize 프로세스 종료
                break
        return result, self.value_table, self.policy_table

    # policy evaluation
    def policy_evaluation(self, iter_num, discount_factor):
        self.discount_factor = discount_factor
        for i in range(iter_num):
            self.policy_evaluate()

    # policy evaluation의 단위 시퀀스
    def policy_evaluate(self):

        # 다음 가치함수 초기화
        next_value_table = []
        max_delta = 0
        for a in range(self.width * self.height * self.passenger_loc_num * self.dest_num):
            next_value_table.append(0.0)

        # 모든 상태에 대해서 벨만 기대방정식을 계산
        for state in self.all_states:
            if self.check_end_state(state):
                next_value_table[self.get_table_index(
                    state[0], state[1], state[2], state[3])] = 20.0

        for state in self.all_states:
            value = 0.0
            # 마침 상태의 가치 함수 = 0
            if self.check_end_state(state):
                continue

            # 벨만 기대 방정식
            for action in self.possible_actions:
                next_state = self.state_after_action(state, action)
                reward = self.get_reward(state, action)
                next_value = self.get_value(next_state)
                value += (self.get_policy(state)[action] *
                          (reward + self.discount_factor * next_value))

            next_value_table[self.get_table_index(
                state[0], state[1], state[2], state[3])] = round(value, 2)
            if (self.get_value(state) - round(value, 2) > max_delta):
                max_delta = self.get_value(state) - round(value, 2) > max_delta

        self.value_table = next_value_table
        return max_delta

    # 현재 가치 함수에 대해서 탐욕 정책 발전
    def policy_improvement(self):
        next_policy = self.policy_table.copy()
        ret = True
        for state in self.all_states:
            if self.check_end_state(state):
                continue
            value = -99999
            max_index = []
            # 반환할 정책 초기화
            result = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

            # 모든 행동에 대해서 [보상 + (감가율 * 다음 상태 가치함수)] 계산
            for index, action in enumerate(self.possible_actions):
                next_state = self.state_after_action(state, action)
                reward = self.get_reward(state, action)
                next_value = self.get_value(next_state)
                temp = reward + self.discount_factor * next_value

                # 받을 보상이 최대인 행동의 index(최대가 복수라면 모두)를 추출
                if temp == value:
                    max_index.append(index)
                elif temp > value:
                    value = temp
                    max_index.clear()
                    max_index.append(index)

            # 행동의 확률 계산
            prob = 1 / len(max_index)

            for index in max_index:
                result[index] = prob

            next_policy[self.get_table_index(
                state[0], state[1], state[2], state[3])] = result

            # for policy stabilize
            if ret:
                if result != self.get_policy(state):
                    ret = False
        self.policy_table = next_policy
        return ret

    # 어떤 state에서 어떤 action을 취한 뒤의 state를 반환
    def state_after_action(self, state, action):
        ret = self.check_state_boundary(state)
        feasible_action = self.feasible_action_in_state(ret)
        if action not in feasible_action:
            pass
        elif action == 0:
            ret[0] = ret[0] + 1
        elif action == 1:
            ret[0] = ret[0] - 1
        elif action == 2:
            ret[1] = ret[1] + 1
        elif action == 3:
            ret[1] = ret[1] - 1
        elif action == 4:
            ret[2] = 4
        elif action == 5:
            ret[2] = self.convert_to_RGYB(ret[0], ret[1])
        return ret

    # State의 RGYB(1 ~ 4)값을 좌표값으로 변환
    def convert_RGYB(self, rgyb: int):
        if rgyb in range(len(self.env.locs)):
            return self.env.locs[rgyb]
        else:
            return None

    # RGYB 값을 좌표로 변환
    def convert_to_RGYB(self, a, b):
        for k in range(len(self.env.locs)):
            if self.env.locs[k] == (a, b):
                return k
        return 5

    # State이 종료상태인지 확인
    def check_end_state(self, state):
        if ((self.convert_RGYB(state[3]) == (state[0], state[1])) and (state[2] == state[3])):
            return True
        return False

    # state의 유효성 검사, 복사된 state 반환
    def check_state_boundary(self, state):
        ret = [0, 0, 0, 0]
        ret[0] = (0 if state[0] < 0 else self.width - 1
                  if state[0] > self.width - 1 else state[0])
        ret[1] = (0 if state[1] < 0 else self.height - 1
                  if state[1] > self.height - 1 else state[1])
        ret[2] = (0 if state[2] < 0 else self.passenger_loc_num - 1
                  if state[2] > self.passenger_loc_num - 1 else state[2])
        ret[3] = state[3]
        # (0 if state[3] < 0 else self.dest_num - 1
        # if state[3] > self.dest_num - 1 else state[3])
        return ret

    # 어떤 상태에서 유의미한 행동에 대한 정보 검색 (이동 방향에 벽이 있거나 유효하지 않은 승하차 시도는 유의미하지 않은 행동으로 분류)
    def feasible_action_in_state(self, state):
        mask = list([0, 0, 0, 0, 0, 0])
        taxi_row, taxi_col, pass_loc, dest_idx = state
        if taxi_row < 4:  # y축 0 ~ 4
            mask[0] = 1  # north 로 이동 가능
        if taxi_row > 0:
            mask[1] = 1  # south 로 이동 가능
        # 장애물이 없다면
        if taxi_col < 4 and self.env.desc[taxi_row + 1, 2 * taxi_col + 2] == b":":
            mask[2] = 1  # east로 이동 가능
        if taxi_col > 0 and self.env.desc[taxi_row + 1, 2 * taxi_col] == b":":
            mask[3] = 1  # west로 이동 가능
        if pass_loc < 4 and (taxi_row, taxi_col) == self.env.locs[pass_loc]:
            mask[4] = 1  # 승객이 탑승한 상태가 아니고, 차량이 승객 위치에 있다면 탑승 가능
        if pass_loc == 4 and (
                (taxi_row, taxi_col) == self.env.locs[dest_idx]
                or (taxi_row, taxi_col) in self.env.locs
        ):
            mask[5] = 1  # 승객이 탑승해 있고, 택시가 마크된 자리에 있다면, 하차 가능
        result = []
        for index, action in enumerate(mask):
            if action == 1:
                result.append(index)
        return result

    # 특정 상태에서 특정 행동을 했을 때 reward 를 반환
    def get_reward(self, state, action):
        next_state = self.state_after_action(state, action)
        if (self.check_end_state(next_state)):
            return (0)
        if (action == 4 and (next_state[2] != 4 or state[2] == 4)):
            return (-10)
        if action == 5:
            return (-10)
        return (-1)

    # value_table, policy_table 의 원소에 접근할 수 있는 인덱스로 state값을 변환
    # a(width), b(height), c(passenger), d(dest)
    def get_table_index(self, a, b, c, d):
        return (d+self.dest_num*(c+self.passenger_loc_num*(b+self.height*(a))))
    # 특정 상태에서 정책에 따른 행동을 반환

    def get_action(self, state):
        # 0 ~ 1 사이의 값을 무작위로 추출
        random_pick = random.randrange(100) / 100
        policy = self.get_policy(state)
        policy_sum = 0.0
        # 정책에 담긴 행동 중에 무작위로 한 행동을 추출
        for index, value in enumerate(policy):
            policy_sum += value
            if random_pick < policy_sum:
                return index

    # 상태에 따른 정책 반환
    def get_policy(self, state):
        return self.policy_table[self.get_table_index(
            state[0], state[1], state[2], state[3])]

    # 가치 함수의 값을 반환
    def get_value(self, state):
        # 소숫점 둘째 자리까지만 계산
        return round(self.value_table[self.get_table_index(
            state[0], state[1], state[2], state[3])], 2)

    # value_table 반환
    def get_values(self):
        return self.value_table

    # 어떤 상태의 value를 value_table 에서 찾아 출력
    def print_value(self, state):
        # 소숫점 둘째 자리까지만 계산
        print("현 상태: ", state, "\nvalue: ", round(self.value_table[self.get_table_index(
            state[0], state[1], state[2], state[3])], 2))
