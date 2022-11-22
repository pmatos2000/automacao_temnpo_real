class Motor
{
	private:
		double velocidade_atual = 0.0;
		double toque_atual = 0.0;
		double tempo_ultima_atualizacao = 0.0;

	public:
		double get_velocidade_atual();
		double atualizar_velocidade(double tensao_entrada);
		Motor();
};