#ifndef MOTOR_HPP_
#define MOTOR_HPP_

class Motor
{
	private:
		double velocidade_referencia = 0.0;
		double velocidade_atual = 0.0;
		double toque_atual = 0.0;
		double tempo_ultima_atualizacao = 0.0;

	public:
		void set_velocidade_referencia(double velocidade_referencia);
		double get_velocidade_atual();
		void atualizar(double tensao_entrada);
		Motor();
};


#endif
