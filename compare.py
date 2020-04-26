import os

input_dir = 'OS_PJ1_Test'
output_dir = 'output'
TIME_UNIT = 1

class TestCase:
        def __init__(self, test_name):
                self.test_name = test_name
                self.input_path = os.path.join(input_dir, f'{test_name}.txt')
                self.stdout_path = os.path.join(output_dir, f'{test_name}_stdout.txt')
                self.dmesg_path = os.path.join(output_dir, f'{test_name}_dmesg.txt')

        def parse_output(self):
                pid_to_name = dict()
                with open(self.stdout_path, 'r') as f:
                        for ln in f.readlines():
                                ln = ln.strip().split()
                                pid_to_name[int(ln[1])] = ln[0]

                mk_proc = lambda name, pid, st, ft: {
                        'name': name,
                        'pid': pid,
                        'start_time': st,
                        'end_time': ft,
                }

                proc_list = []
                with open(self.dmesg_path, 'r') as f:
                        for ln in f.readlines():
                                ln = ln.strip().split()
                                pid = int(ln[3])
                                start_time = float(ln[4])
                                end_time = float(ln[5])
                                proc_list.append(mk_proc(pid_to_name[pid], pid, start_time, end_time))

                for p in proc_list:
                        p['start_time'] = int((p['start_time']) / TIME_UNIT)
                        p['end_time'] = int((p['end_time']) / TIME_UNIT)

                return proc_list

        def simulation(self):
                proc_list = []
                with open(self.input_path, 'r') as f:
                        policy = f.readline().strip()
                        N = int(f.readline().strip())
                        for ln in f.readlines():
                                ln = ln.strip().split()
                                proc_list.append({'index': len(proc_list), 'name': ln[0], 'ready': int(ln[1]), 'exec': int(ln[2])})
                
                proc_list.sort(key=lambda p: p['ready'] * 1000000000 + p['index'])

                if policy == 'FIFO':
                        t = 0
                        for p in proc_list:
                                if p['ready'] <= t:
                                        p['start_time'] = t
                                        p['end_time'] = t + p['exec']
                                        t = p['end_time']
                elif policy == 'RR':
                        t = 0
                        ready_p = 0
                        running_p = -1
                        queue = []
                        while N > 0:
                                while ready_p < N and proc_list[ready_p]['ready'] <= t:
                                        queue.append(ready_p)
                                        ready_p += 1
                                
                                if running_p == -1 and len(queue) > 0:
                                        running_p = queue.pop(0)
                                        RRcnt = 500
                                        if 'start_time' not in proc_list[running_p]:
                                                proc_list[running_p]['start_time'] = t

                                t += 1
                                
                                if running_p != -1:
                                        proc_list[running_p]['exec'] -= 1
                                        RRcnt -= 1
                                        if RRcnt == 0 and proc_list[running_p]['exec'] != 0:
                                                queue.append(running_p)
                                                running_p = -1
                                        elif proc_list[running_p]['exec'] == 0:
                                                proc_list[running_p]['end_time'] = t
                                                N -= 1
                                                running_p = -1
                elif policy == 'SJF':
                        pass
                elif policy == 'PSJF':
                        pass

                return proc_list

        @staticmethod
        def calc_diff(th, rl, name):
                if th[name] == 0:
                        return 0.0;
                prettyfloat = lambda x: '%0.2f' % x
                return prettyfloat((rl[name] - th[name]) / th[name] * 100)

        def compare(self):
                result = f'{self.test_name}.txt: \n'
                theory = sorted(self.simulation(), key=lambda p: p['name'])
                real = sorted(self.parse_output(), key=lambda p: p['name'])
                x = sorted(real, key=lambda p: p['start_time'])[0]
                for i in theory:
                        if i['name'] == x['name']:
                                t0 = x['start_time'] - i['ready']

                for p in real:
                        p['start_time'] -= t0
                        p['end_time'] -= t0

                for th, rl in zip(theory, real):
                        result += f"    Process {th['name']}:\n"
                        result += f"        theory:     start at {th['start_time']}, end at {th['end_time']}\n"
                        result += f"        my_result:  start at {rl['start_time']}, end at {rl['end_time']}\n"
                        result += f"        difference: start_time {self.calc_diff(th, rl, 'start_time')}%, end_time {self.calc_diff(th, rl, 'end_time')}%\n"
                return result

def get_time_unit():
        tc = TestCase('TIME_MEASUREMENT')
        proc_list = tc.parse_output()
        tot = 0.0
        for p in proc_list:
                tot += p['end_time'] - p['start_time']
        return tot / len(proc_list) / 500

if __name__ == '__main__':
        TIME_UNIT = get_time_unit()
        test_case_list = []
        for tp in ['FIFO', 'RR', 'SJF', 'SJF'][0:2]:
                for i in range(1, 6):
                        test_case_list.append(f'{tp}_{i}')
        
        for tc_name in test_case_list:
                tc = TestCase(tc_name)
                result = tc.compare()
                print(result)