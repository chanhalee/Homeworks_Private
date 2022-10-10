import gym
import ale_py
from ale_py.roms import Breakout
from random import randrange
from ale_py import ALEInterface
from policy_iteration import PolicyIteration


def action_mask(state: int, env):
    """Computes an action mask for the action space using the state information."""
    mask = list([0, 0, 0, 0, 0, 0])
    taxi_row, taxi_col, pass_loc, dest_idx = env.decode(state)
    if taxi_row < 4:
        mask[0] = 1
    if taxi_row > 0:
        mask[1] = 1
    if taxi_col < 4 and env.desc[taxi_row + 1, 2 * taxi_col + 2] == b":":
        mask[2] = 1
    if taxi_col > 0 and env.desc[taxi_row + 1, 2 * taxi_col] == b":":
        mask[3] = 1
    if pass_loc < 4 and (taxi_row, taxi_col) == env.locs[pass_loc]:
        mask[4] = 1
    if pass_loc == 4 and (
            (taxi_row, taxi_col) == env.locs[dest_idx]
            or (taxi_row, taxi_col) in env.locs
    ):
        mask[5] = 1
    return mask


env = gym.make("Taxi-v3")
policy_iteration = PolicyIteration(env)
arr = []
observation = env.reset()  # 매 에피소드 시작시 환경을 초기화한다.
policy_iteration.get_ready_for_episode(observation)
for a in range(10):
    policy_iteration.policy_evaluation(10, 0.9)
    policy_iteration.policy_improvement()
print(policy_iteration.get_values())
done = False
for a in range(50):
    env.render()
    state = list(env.decode(observation))
    action = policy_iteration.get_action(state)  # Your agent code here
    observation, reward, done, info = env.step(action)
    if done:
        print("진행 횟수: ", a)
        if reward == 20:
            print("성공 여부: 성공")
        else:
            print("성공 여부: 실패")
        break
env.close()
