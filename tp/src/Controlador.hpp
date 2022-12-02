class Controlador
{
    private:
        double velocidade_referencia = 0;
        double tensao_atual = 0;
    public:
        void set_velocidade_referencia(double velocidade_referencia);
        void atualizar_tensao(double valor_medido);
        double obter_tensao_atual();
};