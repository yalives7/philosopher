// philo.h
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

struct s_data; // forward declaration

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	long long		last_meal;
	int				meals_eaten;
	int is_eating;     // Yemek yiyor mu flag'i
	int full_reported; // Yeme sayısı kontrolü için flag
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				simulation_end;
	int dead_flag;   // Güçlü ölüm kontrolü
	int full_philos; // Doymuş filozof sayısı
	long long		start_time;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
}					t_data;

// main setup
int					parse_args(int ac, char **av, t_data *data);
int					init_all(t_data *data);
void				start_simulation(t_data *data);
void				clean_up(t_data *data);

// time utils
long long			get_timestamp_ms(void);
void				custom_usleep(int ms);

// actions
void				think(t_philo *philo);
void				take_forks(t_philo *philo);
void				eat(t_philo *philo);
void				sleep_philo(t_philo *philo);
void				print_action(t_philo *philo, char *msg);
int					simulation_has_ended(t_data *data);
void				*philosopher_routine(void *arg);

// routine.c fonksiyonları
void				perform_routine(t_philo *philo);

// utils.c içinden gelen fonksiyonlar:
void				smart_sleep(int duration_ms);
int					has_simulation_ended(t_data *data);
int	dead_loop_check(t_data *data); // Gelişmiş ölüm kontrolü
void				print_philo_action(t_philo *philo, const char *status);
void				*monitor_routine(void *arg);

#endif
