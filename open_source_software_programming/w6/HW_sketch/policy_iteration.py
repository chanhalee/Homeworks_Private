import random
from environment import GraphicDisplay, Env


class PolicyIteration:
    def __init__(self, env):
        # 환경에 대한 객체 선언
        self.env = env
        self.height = 5
        self.width = 5
        # 가치함수를 2차원 리스트로 초기화
        self.value_table_pre = [[0.0] * self.width for _ in range(self.height)]
        # 상 하 좌 우 동일한 확률로 정책 초기화
        self.policy_table_pre = [[[0.25, 0.25, 0.25, 0.25, 0, 0]] * self.width
                                 for a in range(self.height)]
        self.value_table_post = [
            [0.0] * self.width for _ in range(self.height)]
        # 상 하 좌 우 동일한 확률로 정책 초기화
        self.policy_table_post = [[[0.25, 0.25, 0.25, 0.25, 0, 0]] * self.width
                                  for a in range(self.height)]
        # 마침 상태의 설정
        # print(list(env.decode(observation)))
        self.passenger_loc = env.locs[list(env.decode(observation))[2]]
        self.dest = env.locs[list(env.decode(observation))[3]]
        self.policy_table_pre[env.locs[list(env.decode(observation))[
            2]][0]][env.locs[list(env.decode(observation))[2]][1]] = [0, 0, 0, 0, 1, 0]
        self.policy_table_post[env.locs[list(env.decode(observation))[
            3]][0]][env.locs[list(env.decode(observation))[3]][1]] = [0, 0, 0, 0, 0, 1]
        self.passenger_on = False
        # 감가율
        self.discount_factor = 0.9

    def policy_evaluation(self):

        # 다음 가치함수 초기화
        next_value_table = [[0.00] * self.width
                            for _ in range(self.height)]

        # 모든 상태에 대해서 벨만 기대방정식을 계산
        for state in self.env.get_all_states():
            value = 0.0
            # 마침 상태의 가치 함수 = 0
            if state == [2, 2]:
                next_value_table[state[0]][state[1]] = value
                continue

            # 벨만 기대 방정식
            for action in self.env.possible_actions:
                next_state = self.env.state_after_action(state, action)
                reward = self.env.get_reward(state, action)
                next_value = self.get_value(next_state)
                value += (self.get_policy(state)[action] *
                          (reward + self.discount_factor * next_value))

            next_value_table[state[0]][state[1]] = round(value, 2)

        self.value_table = next_value_table

    # 현재 가치 함수에 대해서 탐욕 정책 발전
    def policy_improvement(self):
        if (self.passenger_on):
            next_policy = self.policy_table_post
        else:
            next_policy = self.policy_table_pre
        for state in self.env.get_all_states():
            if state == [2, 2]:
                continue
            value = -99999
            max_index = []
            # 반환할 정책 초기화
            result = [0.0, 0.0, 0.0, 0.0]

            # 모든 행동에 대해서 [보상 + (감가율 * 다음 상태 가치함수)] 계산
            for index, action in self.env.possible_actions:
                next_state = self.env.state_after_action(state, action)
                reward = self.env.get_reward(state, action)
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

            next_policy[state[0]][state[1]] = result

        self.policy_table = next_policy

    # 특정 상태에서 정책에 따른 행동을 반환
    def get_action(self, state):
        # 0 ~ 1 사이의 값을 무작위로 추출
        if (self.passenger_on == False and state[0:2] == self.passenger_loc):
            self.passenger_on = True
            return (4)
        if (self.passenger_on and state[0:2] == self.dest):
            return (5)
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
        if self.passenger_on:
            return self.policy_table_post[state[0]][state[1]]
        else:
            return self.policy_table[state[0]][state[1]]

    # 가치 함수의 값을 반환
    def get_value(self, state):
        if self.passenger_on:
            return round(self.value_table_post[state[0]][state[1]], 2)
        # 소숫점 둘째 자리까지만 계산
        return round(self.value_table_pre[state[0]][state[1]], 2)

    def action_mask(self, state: int):
        mask = list([0, 0, 0, 0, 0, 0])
        taxi_row, taxi_col, pass_loc, dest_idx = state
        if taxi_row < 4:
            mask[0] = 1
        if taxi_row > 0:
            mask[1] = 1
        if taxi_col < 4 and self.env.desc[taxi_row + 1, 2 * taxi_col + 2] == b":":
            mask[2] = 1
        if taxi_col > 0 and self.env.desc[taxi_row + 1, 2 * taxi_col] == b":":
            mask[3] = 1
        if pass_loc < 4 and (taxi_row, taxi_col) == self.env.locs[pass_loc]:
            mask[4] = 1
        if pass_loc == 4 and (
                (taxi_row, taxi_col) == self.env.locs[dest_idx]
                or (taxi_row, taxi_col) in self.env.locs
        ):
            mask[5] = 1
        return mask


if __name__ == "__main__":
    env = Env()
    policy_iteration = PolicyIteration(env)
    grid_world = GraphicDisplay(policy_iteration)
    grid_world.mainloop()
